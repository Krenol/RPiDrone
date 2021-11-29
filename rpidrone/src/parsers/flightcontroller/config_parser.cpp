#include "config_parser.hpp"
#include "misc.hpp"

namespace drone
{
    void parseConfigForFlightcontroller(const Config &config, std::string &out) 
    {
        out = CONFIG_TOKEN;
        //controllers
        //roll
        out += (TO_STRING<double>(config.controls.escs.roll_rate.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.k_d, 2)
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.k_aw, 2) 
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_rate.max, 2) + DELIM_ITEM);
        out += (TO_STRING<double>(config.controls.escs.roll_output.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.k_d, 2)
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.k_aw, 2) 
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.roll_output.max, 2) + DELIM_ITEM);
        //pitch
        out += (TO_STRING<double>(config.controls.escs.pitch_rate.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.k_d, 2)
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.k_aw, 2) 
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_rate.max, 2) + DELIM_ITEM);
        out += (TO_STRING<double>(config.controls.escs.pitch_output.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.k_d, 2)
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.k_aw, 2) 
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.pitch_output.max, 2) + DELIM_ITEM);
        //yaw
        out += (TO_STRING<double>(config.controls.escs.yaw_output.k_p, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.k_d, 2)
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.k_i, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.k_aw, 2) 
            + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.min, 2) + DELIM_VAL + TO_STRING<double>(config.controls.escs.yaw_output.max, 2) + DELIM_ITEM);
        //ESC data
        out += DELIM_CAT;
        out += std::to_string(config.controls.escs.min) + DELIM_ITEM + std::to_string(config.controls.escs.max) + DELIM_ITEM;
        out += std::to_string(config.controls.escs.pin_lb) + DELIM_ITEM + std::to_string(config.controls.escs.pin_lf) + DELIM_ITEM
            + std::to_string(config.controls.escs.pin_rb) + DELIM_ITEM + std::to_string(config.controls.escs.pin_rf) + DELIM_ITEM;
        out += config.controls.escs.calibrate ? "1" : "0";
        out += EOL;
    }
}