#include "client_input_parser.hpp"
#include "misc.hpp"

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
        if (JSON_EXISTS(j, "joystick"))
        {
            float offset = i.joystick.offset, degrees = i.joystick.degrees, rot = i.joystick.rotation;
            auto joystick = j.at("joystick");
            if (JSON_EXISTS(joystick, "offset") && JSON_EXISTS(joystick, "degrees"))
            {
                offset = BOUND<float>(joystick.at("offset"), OFFSET_LOWER_BOUND, OFFSET_UPPER_BOUND);
                degrees = BOUND<float>(joystick.at("degrees"), DEGREE_LOWER_BOUND, DEGREE_UPPER_BOUND);
            }
            if (JSON_EXISTS(joystick, "rotation")) {
                rot = BOUND<float>(joystick.at("rotation"), ROTATION_LOWER_BOUND, ROTATION_UPPER_BOUND);
            }
            i.joystick = Joystick(degrees, offset, rot);
        }
    }
    
    void parseThrottleStruct(const nlohmann::json &j, ClientInput &i) 
    {
        if (JSON_EXISTS(j, "throttle"))
        {
            i.throttle = BOUND<int>(j.at("throttle"), THROTTLE_LOWER_BOUND, THROTTLE_UPPER_BOUND);
        } 
    }
    
    void parseGpsStruct(const nlohmann::json &j, ClientInput &i) 
    {
        if (JSON_EXISTS(j, "gps"))
        {
            float longitude = i.gps.longitude, latitude = i.gps.latitude, altitude = i.gps.altitude;
            auto gps = j.at("gps");
            if (JSON_EXISTS(gps, "altitude"))
            {
                altitude = gps.at("altitude");
            }
            if (JSON_EXISTS(gps, "latitude"))
            {
                latitude = gps.at("latitude");
            }
            if (JSON_EXISTS(gps, "longitude"))
            {
                longitude = gps.at("longitude");
            }
            i.gps = GPSCoordinates(altitude, longitude, latitude);
        }
    }
}