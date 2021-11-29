#include "config_parser.hpp"
#include "misc.hpp"

namespace drone
{
    namespace parsers
    {
        namespace flightcontroller
        {
            void ConfigParser::setConfigToken(std::string &out) 
            {
                out = CONFIG_TOKEN;
            }
            
            void ConfigParser::appendNewCategoryToken(std::string &out) 
            {
                out += DELIM_CAT;
            }
            
            void ConfigParser::appendControllerConfigs(const Config &config, std::string &out) 
            {
                appendRollControllerConfigs(config, out);
                appendPitchControllerConfigs(config, out);
                appendYawControllerConfigs(config, out);   
            }
            
            void ConfigParser::appendRollControllerConfigs(const Config &config, std::string &out) 
            {
                out += (TO_STRING<double>(config.controls.escs.roll_rate.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.k_d, 2)
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.k_aw, 2) 
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.max, 2) + DELIM_ITEM);
                out += (TO_STRING<double>(config.controls.escs.roll_output.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.k_d, 2)
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.k_aw, 2) 
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.max, 2) + DELIM_ITEM);
            }
            
            void ConfigParser::appendYawControllerConfigs(const Config &config, std::string &out) 
            {
                out += (TO_STRING<double>(config.controls.escs.yaw_output.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.k_d, 2)
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.k_aw, 2) 
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.max, 2) + DELIM_ITEM);
            }
            
            void ConfigParser::appendPitchControllerConfigs(const Config &config, std::string &out) 
            {
                out += (TO_STRING<double>(config.controls.escs.pitch_rate.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.k_d, 2)
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.k_aw, 2) 
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.max, 2) + DELIM_ITEM);
                out += (TO_STRING<double>(config.controls.escs.pitch_output.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.k_d, 2)
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.k_aw, 2) 
                    + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.max, 2) + DELIM_ITEM);
            }
            
            void ConfigParser::appendEscConfigs(const Config &config, std::string &out) 
            {
                out += std::to_string(config.controls.escs.min) + DELIM_ITEM + std::to_string(config.controls.escs.max) + DELIM_ITEM;
                out += std::to_string(config.controls.escs.pin_lb) + DELIM_ITEM + std::to_string(config.controls.escs.pin_lf) + DELIM_ITEM
                    + std::to_string(config.controls.escs.pin_rb) + DELIM_ITEM + std::to_string(config.controls.escs.pin_rf) + DELIM_ITEM;
                out += config.controls.escs.calibrate ? "1" : "0";
                out += EOL;
            }
        
            void ConfigParser::parseFlightcontrollerConfig(const Config &config, std::string &out) 
            {
                setConfigToken(out);
                appendControllerConfigs(config, out);
                appendNewCategoryToken(out);
                appendEscConfigs(config, out);
            }
        }
    }
}