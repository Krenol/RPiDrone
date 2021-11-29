#include "loop.hpp"
#include <unistd.h>
#include "logs/easylogging++.h"
#include "misc.hpp"
#include "parsers/flightcontroller/input_parser.hpp"
#include "parsers/flightcontroller/output_parser.hpp"
#include "structs/flightcontroller/output.hpp"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network")  //define network log
#if defined(EXEC_TIME_LOG)
#define EXEC_LOG(LEVEL) CLOG(LEVEL, "exec") //define exec time log
#endif
#if defined(FLIGHTCONTROLLER_LOGS)
#define FLIGHT_LOG(LEVEL) CLOG(LEVEL, "flightcontroller")
#endif
#if defined(RPI_LOGS)
#define RPI_LOG(LEVEL) CLOG(LEVEL, "rpi")
#endif

namespace drone
{
    Loop::Loop()
    {
        #if defined(EXEC_TIME_LOG)
        EXEC_LOG(DEBUG) << "datetime;level;t_exec";
        #endif
        #if defined(FLIGHTCONTROLLER_LOGS)
        FLIGHT_LOG(DEBUG) << "datetime;level;yaw;pitch;roll;t_exec;ax;ay;az;rf_t,;rb_t;lf_t;lb_t;throttle";
        #endif
        #if defined(RPI_LOGS)
        RPI_LOG(DEBUG) << "datetime;level;yaw_vel;pitch_angle;roll_angle;throttle";
        #endif
    }

    void Loop::parseAppJson(std::string &msg, FlightcontrollerInput &userInput, const Config &config)
    {
        json j;
        try
        {
            j = json::parse(msg);
            #if defined(NETWORK_DEBUG_LOGS)
            NETWORK_LOG(DEBUG) << "parsed input to json " << j;
            #endif
            if (JSON_EXISTS(j, "disconnected") && j.at("disconnected") == true)
            {
                if (config.logic.motors_off_disconnect)
                {
                    userInput.throttle = 0;
                }
                else
                {
                    userInput.throttle = config.controls.escs.idle;
                }
                userInput.roll_angle = 0;
                userInput.pitch_angle = 0;
                userInput.yaw_vel = 0;
            }
            else
            {
                ClientInput in;
                from_json(j, in);
                userInput = FlightcontrollerInput(in.throttle, in.joystick.offset, in.joystick.degrees, config.controls.max_roll, config.controls.max_pitch, config.controls.max_yawn, in.joystick.rotation, in.gps);
            }
        }
        catch (const std::exception &exc)
        {
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << msg;
        }
    }

    void Loop::loop(Websocket &websocket, drone::Arduino &fc, const Config &config)
    {
        std::string msg, buf = "";
        char out[OUT_MSG_SIZE];
        FlightcontrollerInput userInput;
        FlightcontrollerOutput fc_output;
        GPSCoordinates c;
        json j;
        LOG(INFO) << "starting main control loop";
        #if defined(EXEC_TIME_LOG)
        std::chrono::steady_clock::time_point last_call = std::chrono::steady_clock::now(), now;
        #endif
 
        while(websocket.hasConnection()){
            try
            {
                if (websocket.hasMessages())
                {
                    websocket.getMessage(msg);
                    sendToFlightcontroller(fc, msg, userInput, config);
                }
                if(fc.availableData() > config.flightcontroller.max_serial_buffer) {
                    fc.clearReceiverBuffer();
                }
                fc.serialRead(out, '\n');
                msg = out;
                #if defined(FLIGHTCONTROLLER_LOGS)
                FLIGHT_LOG(INFO) << msg;
                #endif
                parseOutputFromFlightcontroller(msg, fc_output);
                createOutputJson(fc_output.roll_is, fc_output.pitch_is, fc_output.yaw_is, fc_output.roll_should, fc_output.pitch_should, fc_output.yaw_should, c, j);
                msg = j.dump();
                #if defined(NETWORK_DEBUG_LOGS)
                NETWORK_LOG(DEBUG) << "writing " << msg;
                #endif
                websocket.writeMessage(msg);
                
            }
            catch (const std::exception &exc)
            {
                LOG(ERROR) << exc.what();
            }
            #if defined(EXEC_TIME_LOG)
            now = std::chrono::steady_clock::now();
            EXEC_LOG(INFO) << std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call).count();
            last_call = now;
            #endif
        }
        LOG(INFO) << "Connection lost...";
        msg = "{\"disconnected\": true}";
        sendToFlightcontroller(fc, msg, userInput, config);
    }

    void Loop::sendToFlightcontroller(drone::Arduino &fc, std::string &msg, FlightcontrollerInput &userInput, const Config &config) {
        parseAppJson(msg, userInput, config);
        parseControlInputForFlightcontroller(userInput, msg);
        #if defined(RPI_LOGS)
        RPI_LOG(INFO) << msg;
        #endif
        fc.serialSend(msg);
    }

    void Loop::createOutputJson(float roll_is, float pitch_is, float yaw_is, float roll_should, float pitch_should, float yaw_should,const GPSCoordinates &c, json &j)
    {
        j = {
            {"sensors", {
                {"barometric_height", 0}
            }},
            {"position", {
                {"altitude", c.altitude},
                {"latitude", c.latitude},
                {"longitude", c.longitude}
            }},
            {"angles", {
                {"is", {
                    {"yaw", yaw_is},
                    {"pitch", pitch_is},
                    {"roll", roll_is}
                }},
                {"should", {
                    {"yaw", yaw_should},
                    {"pitch", pitch_should},
                    {"roll", roll_should}
                }}
            }}
        };
    }
}