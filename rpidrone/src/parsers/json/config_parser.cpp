#include "config_parser.hpp"


namespace drone
{
    namespace parsers
    {
        namespace json
        {
            void ConfigParser::parseSensorObject(const nlohmann::json &j, structs::config::Config &cfg)
            {
                auto js = j.at("sensors");
                int dec = js.at("decimal_places");
                int calib = js.at("calibration");
                structs::config::GPS gps(js.at("gps").at("port"), js.at("gps").at("baudrate"));
                cfg.sensors = structs::config::SensorsStruct(calib, gps, dec);
            }

            void ConfigParser::parseControlObject(const nlohmann::json &j, structs::config::Config &cfg)
            {
                int min_esc, max_esc, idle, lf, rf, lb, rb;
                float max_p, max_y, max_r;
                double m;
                bool calib;
                nlohmann::json c, val, ki, kd, kp, kaw, diffs;
                std::string pos;
                c = j.at("controls");
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
                c = c.at("controllers");
                ki = c.at("k_i");
                kp = c.at("k_p");
                kd = c.at("k_d");
                diffs = c.at("max_diff");
                kaw = c.at("k_aw");
                m = diffs.at("roll_rate");
                structs::config::PIDControl rac(kp.at("roll_rate"), kd.at("roll_rate"), ki.at("roll_rate"), kaw.at("roll_rate"), -m, m);
                m = diffs.at("pitch_rate");
                structs::config::PIDControl pac(kp.at("pitch_rate"), kd.at("pitch_rate"), ki.at("pitch_rate"), kaw.at("pitch_rate"), -m, m);
                m = diffs.at("roll_output");
                structs::config::PIDControl rrc(kp.at("roll_output"), kd.at("roll_output"), ki.at("roll_output"), kaw.at("roll_output"), -m, m);
                m = diffs.at("pitch_output");
                structs::config::PIDControl prc(kp.at("pitch_output"), kd.at("pitch_output"), ki.at("pitch_output"), kaw.at("pitch_output"), -m, m);
                m = diffs.at("yaw_output");
                structs::config::PIDControl yrc(kp.at("yaw_output"), kd.at("yaw_output"), ki.at("yaw_output"), kaw.at("yaw_output"), -m, m);
                structs::config::Escs escs(rac, pac, rrc, prc, yrc, min_esc, max_esc, idle, lf, rf, lb, rb, calib);
                cfg.controls = structs::config::ControlsStruct(escs, max_p, max_r, max_y);
            }

            void ConfigParser::parseJsonConfigToStruct(const nlohmann::json &j, structs::config::Config &cfg)
            {
                cfg.leds = structs::config::Leds(j.at("leds").at("on_led"), j.at("leds").at("status_led"));
                cfg.logic = structs::config::Logic(j.at("logic").at("motors_off_on_disconnect"), j.at("logic").at("rpi_reset_pin"));
                cfg.server = structs::config::Server(j.at("server").at("port"), j.at("server").at("queue_size"), j.at("server").at("context"), j.at("server").at("ssl"), j.at("server").at("ssl_cert"));
                cfg.queues = structs::config::Queues(j.at("queues").at("read_size"), j.at("queues").at("write_size"));
                cfg.flightcontroller = structs::config::Flightcontroller(j.at("flightcontroller").at("port"), j.at("flightcontroller").at("baudrate"), j.at("flightcontroller").at("max_serial_buffer"));
                parseSensorObject(j, cfg);
                parseControlObject(j, cfg);
            }
        }
    }
}