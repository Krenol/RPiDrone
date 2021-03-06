#include "loop.hpp"
#include <unistd.h>
#include <fstream>
#include "logs/easylogging++.h"
#include "misc.hpp"
#include "wiringSerial.h"
#include "parsers/arduino_config_parser.hpp"
#include "parsers/arduino_input_parser.hpp"
#include "parsers/arduino_output_parser.hpp"
#include "structs/output.hpp"

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
    Loop::Loop(const std::string &config_path)
    {
        std::string conf;
        loadConfig(config_path);
        server_ = std::make_unique<rpisocket::WiFiServer>(config_.server.port, config_.server.bytes);
        fc_ = std::make_unique<drone::Arduino>(config_.flightcontroller.port, config_.flightcontroller.baudrate);
        parse_config(config_, conf);
        fc_->init(conf);
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

    Loop::~Loop()
    {
        server_->disconnect();
    }

    void Loop::parseUserInput(std::string &msg, UserInput &userInput)
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
                if (config_.logic.motors_off_disconnect)
                {
                    userInput.throttle = 0;
                }
                else
                {
                    userInput.throttle = config_.controls.escs.idle;
                }
                userInput.roll_angle = 0;
                userInput.pitch_angle = 0;
                userInput.yaw_vel = 0;
            }
            else
            {
                Input in;
                from_json(j, in);
                userInput = UserInput(in.throttle, in.joystick.offset, in.joystick.degrees, config_.controls.max_roll, config_.controls.max_pitch, config_.controls.max_yawn, in.joystick.rotation, in.gps);
            }
        }
        catch (const std::exception &exc)
        {
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << msg;
        }
    }

    bool Loop::parseBuffer(std::string &buf, std::string &msg) {
        std::size_t pos;
        if ((pos = buf.find(config_.server.delimiter)) != std::string::npos)
        {
            msg = buf.substr(0, pos);
            buf.erase(0, pos + config_.server.delimiter.length());
            pos = msg.find_first_of("{");
            msg.erase(0, pos);
            return true;
        }
        return false;
    }

    void Loop::readFromSocket(std::string &buf, int max_iter) 
    {
        std::string msg;
        do 
        {
            server_->readBytes(msg);
            buf += msg;
            if(buf.find(config_.server.delimiter) != std::string::npos) return;
            --max_iter;
        } while (max_iter > 0);
    }

    void Loop::loop()
    {
        std::string msg, buf = "";
        char out[OUT_MSG_SIZE];
        UserInput userInput;
        Output output_struct;
        GPSCoordinates c;
        json j;
        LOG(INFO) << "starting main control loop";
        #if defined(EXEC_TIME_LOG)
        std::chrono::steady_clock::time_point last_call = std::chrono::steady_clock::now(), now;
        #endif
        while (1)
        {
            server_->connect(); //use async?
            while(server_->hasConnection()){
                try
                {
                    if (server_->hasData())
                    {
                        readFromSocket(buf, 3);
                        LOG(INFO) << buf;
                    }
                    if(parseBuffer(buf, msg)) 
                    {
                        sendToFlightcontroller(msg, userInput);
                    }
                    if(fc_->availableData() > config_.flightcontroller.max_serial_buffer) {
                        fc_->clearReceiverBuffer();
                    }
                    fc_->serialRead(out, '\n');
                    msg = out;
                    #if defined(FLIGHTCONTROLLER_LOGS)
                    FLIGHT_LOG(INFO) << msg;
                    #endif
                    parse_output(msg, output_struct);
                    createOutputJson(output_struct.roll, output_struct.pitch, output_struct.yaw, c, j);
                    msg = j.dump();
                    #if defined(NETWORK_DEBUG_LOGS)
                    NETWORK_LOG(DEBUG) << "writing " << msg;
                    #endif
                    server_->writeBytes(msg + config_.server.delimiter);
                    
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
            msg = "{\"disconnected\": true}";
            sendToFlightcontroller(msg, userInput);
            clearReceiveBuffer(fd_ard_);
        }
    }

    void Loop::sendToFlightcontroller(std::string &msg, UserInput &userInput) {
        parseUserInput(msg, userInput);
        parse_input(userInput, msg);
        #if defined(RPI_LOGS)
        RPI_LOG(INFO) << msg;
        #endif
        fc_->serialSend(msg);
    }

    void Loop::createOutputJson(float roll, float pitch, float yaw, const GPSCoordinates &c, json &j)
    {
        j = {
            {"angles", {{"yaw", yaw}, {"pitch", pitch}, {"roll", roll}}},
            {"position", {{"latitude", c.latitude}, {"longitude", c.longitude}, {"altitude", c.altitude}}},
            {"sensors", {{"barometric_height", 0}}}};
    }

    void Loop::loadConfig(const std::string &file)
    {
        std::ifstream ifs(file);
        config_ = json::parse(ifs);
    }
}