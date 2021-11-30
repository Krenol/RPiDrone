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
            
            void ConfigParser::appendValueDelim(std::string &out) 
            {
                out += DELIM_VAL;
            }
            
            void ConfigParser::appendItemDelim(std::string &out) 
            {
                out += DELIM_ITEM;
            }
            
            void ConfigParser::appendEOL(std::string &out) 
            {
                out += EOL;
            }
            
            void ConfigParser::appendNewCategoryToken(std::string &out) 
            {
                out += DELIM_CAT;
            }
            
            void ConfigParser::appendControllerConfigs(const Config &config, std::string &out) 
            {
                appendRollControllerConfigs(config, out);
                appendItemDelim(out);
                appendPitchControllerConfigs(config, out);
                appendItemDelim(out);
                appendYawControllerConfigs(config, out);   
            }
            
            void ConfigParser::appendRollControllerConfigs(const Config &config, std::string &out) 
            {
                appendRollRateControllerConfigs(config, out);
                appendItemDelim(out);
                appendRollOutControllerConfigs(config, out);                
            }
            
            void ConfigParser::appendRollRateControllerConfigs(const Config &config, std::string &out) 
            {
                out += TO_STRING<double>(config.controls.escs.roll_rate.k_p, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_rate.k_d, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_rate.k_i, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_rate.k_aw, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_rate.min, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_rate.max, 2);
            }
            
            void ConfigParser::appendRollOutControllerConfigs(const Config &config, std::string &out) 
            {
                out += TO_STRING<double>(config.controls.escs.roll_output.k_p, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_output.k_d, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_output.k_i, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_output.k_aw, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_output.min, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.roll_output.max, 2);
            }
            
            void ConfigParser::appendYawControllerConfigs(const Config &config, std::string &out) 
            {
                //yaw is already a velocity; we only have the output controller
                out += TO_STRING<double>(config.controls.escs.yaw_output.k_p, 2); 
                appendValueDelim(out); out += TO_STRING<double>(config.controls.escs.yaw_output.k_d, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.yaw_output.k_i, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.yaw_output.k_aw, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.yaw_output.min, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.yaw_output.max, 2);
            }
            
            void ConfigParser::appendPitchControllerConfigs(const Config &config, std::string &out) 
            {
                appendPitchRateControllerConfigs(config, out);               
                appendItemDelim(out);
                appendPitchOutControllerConfigs(config, out);
            }
            
            void ConfigParser::appendPitchRateControllerConfigs(const Config &config, std::string &out) 
            {
                out += TO_STRING<double>(config.controls.escs.pitch_rate.k_p, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_rate.k_d, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_rate.k_i, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_rate.k_aw, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_rate.min, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_rate.max, 2);
            }
            
            void ConfigParser::appendPitchOutControllerConfigs(const Config &config, std::string &out) 
            {
                out += TO_STRING<double>(config.controls.escs.pitch_output.k_p, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_output.k_d, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_output.k_i, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_output.k_aw, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_output.min, 2); 
                appendValueDelim(out); 
                out += TO_STRING<double>(config.controls.escs.pitch_output.max, 2);
            }
            
            void ConfigParser::appendEscConfigs(const Config &config, std::string &out) 
            {
                out += std::to_string(config.controls.escs.min);
                appendItemDelim(out);
                out += std::to_string(config.controls.escs.max);
                appendItemDelim(out);
                out += std::to_string(config.controls.escs.pin_lb);
                appendItemDelim(out);
                out += std::to_string(config.controls.escs.pin_lf);
                appendItemDelim(out);
                out += std::to_string(config.controls.escs.pin_rb);
                appendItemDelim(out);
                out += std::to_string(config.controls.escs.pin_rf);
                appendItemDelim(out);
                out += config.controls.escs.calibrate ? "1" : "0";
            }
        
            void ConfigParser::parseFlightcontrollerConfig(const Config &config, std::string &out) 
            {
                setConfigToken(out);
                appendControllerConfigs(config, out);
                appendNewCategoryToken(out);
                appendEscConfigs(config, out);
                appendEOL(out);
            }
        }
    }
}