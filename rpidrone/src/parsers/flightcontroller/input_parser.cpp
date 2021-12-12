#include "input_parser.hpp"
#include "misc/string.hpp"

namespace drone
{
    namespace parsers
    {
        namespace flightcontroller
        {
            void InputParser::setInputToken(std::string &out) 
            {
                out = INPUT_TOKEN;
            }
            
            void InputParser::appendDelim(std::string &out) 
            {
                out += DELIM_ITEM;
            }
            
            void InputParser::appendEOL(std::string &out) 
            {
                out += EOL;
            }
            
            void InputParser::appendYPRAngles(const FlightcontrollerInput &input, std::string &out) 
            {
                out += misc::StringParser<float>::roundToString(input.yaw_vel, 1);
                appendDelim(out);
                out += misc::StringParser<float>::roundToString(input.pitch_angle, 1);
                appendDelim(out);
                out += misc::StringParser<float>::roundToString(input.roll_angle, 1);
            }
            
            void InputParser::appendThrottleValue(const FlightcontrollerInput &input, std::string &out) 
            {
                out += std::to_string(input.throttle);
            }
        
            void InputParser::parseFlightcontrollerInput(const FlightcontrollerInput &input, std::string &out) 
            {
                setInputToken(out);
                appendYPRAngles(input, out);
                appendDelim(out);
                appendThrottleValue(input, out);
                appendEOL(out);
            }
        }
    }
}