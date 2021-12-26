#include "client_input_parser.hpp"
#include "misc/numeric.hpp"
#include "misc/json.hpp"
#include "constants/input.hpp"

namespace drone
{
    namespace parsers
    {
        namespace json
        {
            void InputParser::parseJoystickStruct(const nlohmann::json &json, Input &clientInput)
            {
                if (misc::Json::jsonFieldExists(json, "joystick"))
                {
                    float offset = clientInput.joystick.offset, degrees = clientInput.joystick.degrees, rot = clientInput.joystick.rotation;
                    auto joystick = json.at("joystick");
                    if (misc::Json::jsonFieldExists(joystick, "offset") && misc::Json::jsonFieldExists(joystick, "degrees"))
                    {
                        offset = misc::Numeric<float>::bound(joystick.at("offset"), constants::clientinput::OFFSET_LOWER_BOUND, constants::clientinput::OFFSET_UPPER_BOUND);
                        degrees = misc::Numeric<float>::bound(joystick.at("degrees"), constants::clientinput::DEGREE_LOWER_BOUND, constants::clientinput::DEGREE_UPPER_BOUND);
                    }
                    if (misc::Json::jsonFieldExists(joystick, "rotation"))
                    {
                        rot = misc::Numeric<float>::bound(joystick.at("rotation"), constants::clientinput::ROTATION_LOWER_BOUND, constants::clientinput::ROTATION_UPPER_BOUND);
                    }
                    clientInput.joystick = Joystick(degrees, offset, rot);
                }
            }

            void InputParser::parseThrottleStruct(const nlohmann::json &json, Input &clientInput)
            {
                if (misc::Json::jsonFieldExists(json, "throttle"))
                {
                    clientInput.throttle = misc::Numeric<int>::bound(json.at("throttle"), constants::clientinput::THROTTLE_LOWER_BOUND, constants::clientinput::THROTTLE_UPPER_BOUND);
                }
            }

            void InputParser::parseGpsStruct(const nlohmann::json &json, Input &clientInput)
            {
                if (misc::Json::jsonFieldExists(json, "gps"))
                {
                    float longitude = clientInput.gps.longitude, latitude = clientInput.gps.latitude, altitude = clientInput.gps.altitude;
                    auto gps = json.at("gps");
                    if (misc::Json::jsonFieldExists(gps, "altitude"))
                    {
                        altitude = gps.at("altitude");
                    }
                    if (misc::Json::jsonFieldExists(gps, "latitude"))
                    {
                        latitude = gps.at("latitude");
                    }
                    if (misc::Json::jsonFieldExists(gps, "longitude"))
                    {
                        longitude = gps.at("longitude");
                    }
                    clientInput.gps = GPSCoordinates(altitude, longitude, latitude);
                }
            }
            
            void InputParser::parseInputJsonToStruct(const nlohmann::json &json, Input &clientInput) 
            {
                parseThrottleStruct(json, clientInput);
                parseJoystickStruct(json, clientInput);
                parseGpsStruct(json, clientInput);
            }
        }
    }
}