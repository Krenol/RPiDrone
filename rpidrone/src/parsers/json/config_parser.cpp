#include "config_parser.hpp"


namespace drone
{
    namespace parsers
    {
        namespace json
        {
            nlohmann::json ConfigParser::getSensorsJsonObject(const nlohmann::json &configJson) 
            {
                return configJson.at("sensors");
            }
            
            nlohmann::json ConfigParser::getGpsJsonObject(const nlohmann::json &configJson) 
            {
                return configJson.at("sensors").at("gps");
            }
        
            nlohmann::json ConfigParser::getDroneControlsJsonObject(const nlohmann::json &configJson) 
            {
                return configJson.at("controls");
            }
        
            nlohmann::json ConfigParser::getControllersJsonObject(const nlohmann::json &configJson) 
            {
                return configJson.at("controls").at("controllers");
            }
        
            void ConfigParser::setSensorsStruct(const nlohmann::json &configJson, structs::config::Config &cfg)
            {
                structs::config::GPS gps = getGpsStruct(configJson);
                auto sensors = getSensorsJsonObject(configJson);
                int decimalPlaces = sensors.at("decimal_places");
                bool calibration = sensors.at("calibration");
                cfg.sensors = structs::config::SensorsStruct(calibration, gps, decimalPlaces);
            }
            
            structs::config::GPS ConfigParser::getGpsStruct(const nlohmann::json &configJson) 
            {
                auto gpsJson = getGpsJsonObject(configJson);
                return structs::config::GPS(gpsJson.at("port"), gpsJson.at("baudrate"));
            }

            void ConfigParser::setControlsStruct(const nlohmann::json &configJson, structs::config::Config &cfg)
            {
                int min_esc, max_esc, idle, lf, rf, lb, rb;
                float max_p, max_y, max_r;
                double m;
                bool calib;
                nlohmann::json c, val, ki, kd, kp, kaw, diffs;
                std::string pos;
                c = configJson.at("controls");
                max_p = c.at("max_pitch_rate");
                max_r = c.at("max_roll_rate");
                max_y = c.at("max_yaw_velocity");
                c = c.at("escs");
                calib = c.at("calibrate");
                min_esc = c.at("min");
                max_esc = c.at("max");
                idle = c.at("idle");
                auto esc_pins = c.at("pins");
                for (auto esc : esc_pins.items())
                {
                    val = esc.value();
                    pos = val.at("pos");
                    if (pos == "rb")
                    {
                        rb = val.at("pin");
                    }
                    else if (pos == "lb")
                    {
                        lb = val.at("pin");
                    }
                    else if (pos == "rf")
                    {
                        rf = val.at("pin");
                    }
                    else if (pos == "lf")
                    {
                        lf = val.at("pin");
                    }
                }
                structs::config::EscControllers escControllers = getEscControllers(configJson);
                structs::config::Escs escs(escControllers, min_esc, max_esc, idle, lf, rf, lb, rb, calib);
                cfg.controls = structs::config::ControlsStruct(escs, max_p, max_r, max_y);
            }
            
            structs::config::EscControllers ConfigParser::getEscControllers(const nlohmann::json &configJson) 
            {
                auto controllersJson = getControllersJsonObject(configJson);
                auto rollRateController = parsePidControlStruct(controllersJson, "roll_rate");
                auto pitchRateController = parsePidControlStruct(controllersJson, "pitch_rate");
                auto rollOutputController = parsePidControlStruct(controllersJson, "roll_output");
                auto pitchOutputController = parsePidControlStruct(controllersJson, "pitch_output");
                auto yawOutputController = parsePidControlStruct(controllersJson, "yaw_output");
                structs::config::EscControllers escControllers(rollRateController, pitchRateController, rollOutputController, pitchOutputController, yawOutputController);
            }
            
            structs::config::PID ConfigParser::parsePidControlStruct(const nlohmann::json &controllersJson, const std::string &key) 
            {
                
                double maxOffset = controllersJson.at("max_diff").at(key);
                double kp = controllersJson.at("k_p").at(key);
                double ki = controllersJson.at("k_i").at(key);
                double kd = controllersJson.at("k_d").at(key);
                double kaw = controllersJson.at("k_aw").at(key);
                return structs::config::PID(kp, kd, ki, kaw, -maxOffset, maxOffset);
            }

            void ConfigParser::parseJsonConfigToStruct(const nlohmann::json &configJson, structs::config::Config &cfg)
            {
                cfg.leds = structs::config::Leds(configJson.at("leds").at("on_led"), configJson.at("leds").at("status_led"));
                cfg.logic = structs::config::Logic(configJson.at("logic").at("motors_off_on_disconnect"), configJson.at("logic").at("rpi_reset_pin"));
                cfg.server = structs::config::Server(configJson.at("server").at("port"), configJson.at("server").at("queue_size"), configJson.at("server").at("context"), configJson.at("server").at("ssl"), configJson.at("server").at("ssl_cert"));
                cfg.queues = structs::config::Queues(configJson.at("queues").at("read_size"), configJson.at("queues").at("write_size"));
                cfg.flightcontroller = structs::config::Flightcontroller(configJson.at("flightcontroller").at("port"), configJson.at("flightcontroller").at("baudrate"), configJson.at("flightcontroller").at("max_serial_buffer"));
                setSensorsStruct(configJson, cfg);
                setControlsStruct(configJson, cfg);
            }
        }
    }
}