#include "input_parser.hpp"
#include "misc/string.hpp"
#include "constants/flightcontroller.hpp"
#include "constants/client_input.hpp"
#include <cmath>

namespace drone
{
    namespace parsers
    {
        namespace flightcontroller
        {
            void InputParser::setInputToken(std::string &out) 
            {
                out = constants::flightcontroller::INPUT_TOKEN;
            }
            
            void InputParser::appendDelim(std::string &out) 
            {
                out += constants::flightcontroller::ITEM_DELIMITER;
            }
            
            void InputParser::appendEOL(std::string &out) 
            {
                out += constants::flightcontroller::EOL;
            }
            
            void InputParser::appendYPRAngles(const structs::middleware::Input &input, std::string &out, const structs::config::Config &config) 
            {
                float roll_angle = calculateShouldRollAngle(input, config);
                float pitch_angle = calculateShouldPitchAngle(input, config);
                float yaw_vel = calculateShouldYawVelocity(input, config);
                out += misc::StringParser<float>::roundToString(yaw_vel, 1);
                appendDelim(out);
                out += misc::StringParser<float>::roundToString(pitch_angle, 1);
                appendDelim(out);
                out += misc::StringParser<float>::roundToString(roll_angle, 1);
            }
            
            float InputParser::calculateShouldRollAngle(const structs::middleware::Input &input, const structs::config::Config &config) 
            {
                return input.joystick.offset * cos(input.joystick.degrees * M_PI / 180.0) * config.controls.maxYprRates.maxRollRate;
            }
            
            float InputParser::calculateShouldPitchAngle(const structs::middleware::Input &input, const structs::config::Config &config) 
            {
                return input.joystick.offset * sin(input.joystick.degrees * M_PI / 180.0) * config.controls.maxYprRates.maxPitchRate;
            }
            
            float InputParser::calculateShouldYawVelocity(const structs::middleware::Input &input, const structs::config::Config &config) 
            {
                return config.controls.maxYprRates.maxYawVel * input.joystick.rotation / drone::constants::clientinput::ROTATION_UPPER_BOUND;
            }
            
            void InputParser::appendThrottleValue(const structs::middleware::Input &input, std::string &out) 
            {
                out += std::to_string(input.throttle);
            }
        
            void InputParser::parseFlightcontrollerInput(const structs::middleware::Input &input, std::string &out, const structs::config::Config &config) 
            {
                setInputToken(out);
                appendYPRAngles(input, out, config);
                appendDelim(out);
                appendThrottleValue(input, out);
                appendEOL(out);
            }
        }
    }
}