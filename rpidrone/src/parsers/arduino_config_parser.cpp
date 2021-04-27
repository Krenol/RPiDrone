#include "arduino_config_parser.hpp"

namespace drone
{
    void parse_config(const Config &config, std::string &out) 
    {
        out = CONFIG_TOKEN;
        //controllers
        //roll
        out += (std::to_string(config.controls.escs.roll_rate.k_p) + DELIM_VAL + std::to_string(config.controls.escs.roll_rate.k_d)
            + DELIM_VAL + std::to_string(config.controls.escs.roll_rate.k_i) + DELIM_VAL + std::to_string(config.controls.escs.roll_rate.k_aw) 
            + DELIM_VAL + std::to_string(config.controls.escs.roll_rate.min) + DELIM_VAL + std::to_string(config.controls.escs.roll_rate.max) + DELIM_ITEM);
        out += (std::to_string(config.controls.escs.roll_output.k_p) + DELIM_VAL + std::to_string(config.controls.escs.roll_output.k_d)
            + DELIM_VAL + std::to_string(config.controls.escs.roll_output.k_i) + DELIM_VAL + std::to_string(config.controls.escs.roll_output.k_aw) 
            + DELIM_VAL + std::to_string(config.controls.escs.roll_output.min) + DELIM_VAL + std::to_string(config.controls.escs.roll_output.max) + DELIM_ITEM);
        //pitch
        out += (std::to_string(config.controls.escs.pitch_rate.k_p) + DELIM_VAL + std::to_string(config.controls.escs.pitch_rate.k_d)
            + DELIM_VAL + std::to_string(config.controls.escs.pitch_rate.k_i) + DELIM_VAL + std::to_string(config.controls.escs.pitch_rate.k_aw) 
            + DELIM_VAL + std::to_string(config.controls.escs.pitch_rate.min) + DELIM_VAL + std::to_string(config.controls.escs.pitch_rate.max) + DELIM_ITEM);
        out += (std::to_string(config.controls.escs.pitch_output.k_p) + DELIM_VAL + std::to_string(config.controls.escs.pitch_output.k_d)
            + DELIM_VAL + std::to_string(config.controls.escs.pitch_output.k_i) + DELIM_VAL + std::to_string(config.controls.escs.pitch_output.k_aw) 
            + DELIM_VAL + std::to_string(config.controls.escs.pitch_output.min) + DELIM_VAL + std::to_string(config.controls.escs.pitch_output.max) + DELIM_ITEM);
        //yaw
        out += (std::to_string(config.controls.escs.yaw_output.k_p) + DELIM_VAL + std::to_string(config.controls.escs.yaw_output.k_d)
            + DELIM_VAL + std::to_string(config.controls.escs.yaw_output.k_i) + DELIM_VAL + std::to_string(config.controls.escs.yaw_output.k_aw) 
            + DELIM_VAL + std::to_string(config.controls.escs.yaw_output.min) + DELIM_VAL + std::to_string(config.controls.escs.yaw_output.max) + DELIM_ITEM);
        //ESC data
        out += DELIM_CAT;
        out += std::to_string(config.controls.escs.min) + DELIM_ITEM + std::to_string(config.controls.escs.max) + DELIM_ITEM;
        out += std::to_string(config.controls.escs.pin_lb) + DELIM_ITEM + std::to_string(config.controls.escs.pin_lf) + DELIM_ITEM
            + std::to_string(config.controls.escs.pin_rb) + DELIM_ITEM + std::to_string(config.controls.escs.pin_rf) + DELIM_ITEM;
        out += config.controls.escs.calibrate ? "1" : "0";
        out += EOL;
    }
}