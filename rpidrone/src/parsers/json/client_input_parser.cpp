#include "client_input_parser.hpp"
#include "misc/numeric.hpp"
#include "misc/json.hpp"

namespace drone
{
    void from_json(const nlohmann::json &j, ClientInput &i) 
    {
        parseThrottleStruct(j, i);
        parseJoystickStruct(j,i);
        parseGpsStruct(j,i);
    }
    
    void parseJoystickStruct(const nlohmann::json &j, ClientInput &i) 
    {
        if (misc::Json::jsonFieldExists(j, "joystick"))
        {
            float offset = i.joystick.offset, degrees = i.joystick.degrees, rot = i.joystick.rotation;
            auto joystick = j.at("joystick");
            if (misc::Json::jsonFieldExists(joystick, "offset") && misc::Json::jsonFieldExists(joystick, "degrees"))
            {
                offset = misc::Numeric<float>::bound(joystick.at("offset"), OFFSET_LOWER_BOUND, OFFSET_UPPER_BOUND);
                degrees = misc::Numeric<float>::bound(joystick.at("degrees"), DEGREE_LOWER_BOUND, DEGREE_UPPER_BOUND);
            }
            if (misc::Json::jsonFieldExists(joystick, "rotation")) {
                rot = misc::Numeric<float>::bound(joystick.at("rotation"), ROTATION_LOWER_BOUND, ROTATION_UPPER_BOUND);
            }
            i.joystick = Joystick(degrees, offset, rot);
        }
    }
    
    void parseThrottleStruct(const nlohmann::json &j, ClientInput &i) 
    {
        if (misc::Json::jsonFieldExists(j, "throttle"))
        {
            i.throttle = misc::Numeric<int>::bound(j.at("throttle"), THROTTLE_LOWER_BOUND, THROTTLE_UPPER_BOUND);
        } 
    }
    
    void parseGpsStruct(const nlohmann::json &j, ClientInput &i) 
    {
        if (misc::Json::jsonFieldExists(j, "gps"))
        {
            float longitude = i.gps.longitude, latitude = i.gps.latitude, altitude = i.gps.altitude;
            auto gps = j.at("gps");
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
            i.gps = GPSCoordinates(altitude, longitude, latitude);
        }
    }
}