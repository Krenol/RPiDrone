#include "loop.hpp"
#include <unistd.h>
#include <fstream>
#include <future>
#include "logs/easylogging++.h"
#include "misc.hpp"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network")  //define network log
#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define control log
#if defined(EXEC_TIME_LOG)
#define EXEC_LOG(LEVEL) CLOG(LEVEL, "exec") //define exec time log
#endif

namespace drone
{
    Loop::Loop(const std::string &config_path)
    {
        loadConfig(config_path);
        //pin::initGPIOs();
        connection_ = std::make_unique<Connection>(std::make_unique<rpisocket::WiFiServer>(config_.server.port, config_.server.bytes), config_.server.delimiter, config_.queues.read_size);
        //on_led_ = std::make_unique<rpicomponents::Led>(config_.leds.on_led, pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        //status_led_ = std::make_unique<rpicomponents::Led>(config_.leds.status_led, pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        tpe_ = std::make_unique<design_patterns::ThreadPoolExecutor>(1, config_.queues.write_size);

        #if defined(EXEC_TIME_LOG)
        EXEC_LOG(DEBUG) << "datetime;level;t_exec";
        #endif
    }

    Loop::~Loop()
    {
        //on_led_->TurnOff();
        //status_led_->TurnOff();
        connection_->stopThread();
        //pin::terminateGPIOs();
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
                //int throttle, int idle, float offset, float degrees, float max_roll, float max_pitch, float max_yawn, float rotation, GPSCoordinates &gps)
                userInput = UserInput(in.throttle, config_.controls.escs.idle, config_.controls.escs.max, in.joystick.offset, in.joystick.degrees, config_.controls.max_roll, config_.controls.max_pitch, config_.controls.max_yawn, in.joystick.rotation, in.gps);
            }
        }
        catch (const std::exception &exc)
        {
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << msg;
        }
    }

    void Loop::loop()
    {
        std::string delimiter = config_.server.delimiter, msg;
        UserInput userInput;
        connection_->startThread();
        CONTROL_LOG(INFO) << "starting main control loop";
        #if defined(EXEC_TIME_LOG)
        std::chrono::steady_clock::time_point last_call = std::chrono::steady_clock::now(), now;
        #endif
        while (1)
        {
            try
            {
                if (connection_->hasItem())
                {
                    connection_->pop(msg);
                    parseUserInput(msg, userInput);
                }
                tpe_->enqueue([this, &delimiter]() {
                    if (connection_->hasConnection())
                    {
                        GPSCoordinates c;
                        json j;
                        createOutputJson(1, 1, c, j);
                        std::string msg = j.dump();
                        #if defined(NETWORK_DEBUG_LOGS)
                        NETWORK_LOG(DEBUG) << "writing " << msg;
                        #endif
                        connection_->writeMsg(msg + delimiter);
                    }
                });
            }
            catch (const std::exception &exc)
            {
                CONTROL_LOG(ERROR) << exc.what();
            }
            #if defined(EXEC_TIME_LOG)
            now = std::chrono::steady_clock::now();
            EXEC_LOG(INFO) << std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call).count();
            last_call = now;
            #endif
        }
    }

    void Loop::createOutputJson(float roll, float pitch, const GPSCoordinates &c, json &j)
    {
        j = {
            {"angles", {{"yaw", 0}, {"pitch", pitch}, {"roll", roll}}},
            {"position", {{"latitude", c.latitude}, {"longitude", c.longitude}, {"altitude", c.altitude}}},
            {"sensors", {{"barometric_height", 0}}}};
    }

    void Loop::loadConfig(const std::string &file)
    {
        std::ifstream ifs(file);
        config_ = json::parse(ifs);
    }
}