#include "json_parser.hpp"
#include "easylogging++.h"
#include "misc.hpp"

namespace drone
{
    // const float DEGREE_UPPER_BOUND = 360.0f, OFFSET_UPPER_BOUND = 1.0f, DEGREE_LOWER_BOUND = -360.0f, OFFSET_LOWER_BOUND = 0.0f, ROTATION_UPPER_BOUND = 100.0f, ROTATION_LOWER_BOUND = -100.0f;
    // const int THROTTLE_UPPER_BOUND = 100, THROTTLE_LOWER_BOUND = 0;
    
    void from_json(const nlohmann::json &j, Config &cfg) 
    {
        cfg.leds = Leds(j.at("leds").at("on_led"), j.at("leds").at("status_led"));
        cfg.logic = Logic(j.at("logic").at("main_loop_sleep"), j.at("logic").at("motors_off_on_disconnect"));
        cfg.server = Server(j.at("server").at("port"), j.at("server").at("bytes"), j.at("server").at("delimiter"));
        cfg.queues = Queues(j.at("queues").at("read_size"), j.at("queues").at("write_size"));
        parse_sensor_obj(j, cfg);
        parse_control_obj(j, cfg);
    }
    
    void from_json(const nlohmann::json &j, Input &i) 
    {
        int throttle = i.throttle;
        float offset = i.joystick.offset, degrees = i.joystick.degrees, rot = i.joystick.rotation, longitude = i.gps.longitude, latitude = i.gps.latitude, altitude = i.gps.altitude;
        if (JSON_EXISTS(j, "throttle"))
        {
            throttle = BOUND<int>(j.at("throttle"), THROTTLE_LOWER_BOUND, THROTTLE_UPPER_BOUND);
        } 

        if (JSON_EXISTS(j, "joystick"))
        {
            auto joystick = j.at("joystick");
            if (JSON_EXISTS(joystick, "offset") && JSON_EXISTS(joystick, "degrees"))
            {
                offset = BOUND<float>(joystick.at("offset"), OFFSET_LOWER_BOUND, OFFSET_UPPER_BOUND);
                degrees = BOUND<float>(joystick.at("degrees"), DEGREE_LOWER_BOUND, DEGREE_UPPER_BOUND);
            }
            if (JSON_EXISTS(j, "rotation")) {
                rot = BOUND<float>(joystick.at("rotation"), ROTATION_LOWER_BOUND, ROTATION_UPPER_BOUND);
            }
        }

        if (JSON_EXISTS(j, "gps"))
        {
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
        }

        i.throttle = throttle;
        i.gps = rpicomponents::GPSCoordinates(altitude, longitude, latitude);
        i.joystick = Joystick(degrees, offset, rot);
    }
    
    void parse_sensor_obj(const nlohmann::json &j, Config &cfg) 
    {
        auto js = j.at("sensors");
        int dec = js.at("decimal_places");
        SensorCalibration sc(js.at("calibration").at("calibrate"), js.at("calibration").at("measurements"));
        GPS gps(js.at("gps").at("port"), js.at("gps").at("baudrate"));
        auto b = js.at("bmp");
        BMPKalman bkal(b.at("kalman").at("c1"), b.at("kalman").at("c2"), b.at("kalman").at("q11"), b.at("kalman").at("q12"), b.at("kalman").at("q21"), b.at("kalman").at("q22"));
        BMP bmp(b.at("address"), b.at("accuracy"), bkal);
        b = js.at("mpu");
        js = b.at("kalman");
        MPUKalmanAngles ma(js.at("angles").at("c1"), js.at("angles").at("c2"), js.at("angles").at("r"), js.at("angles").at("q11"), js.at("angles").at("q12"), js.at("angles").at("q21"), js.at("angles").at("q22"));
        MPUKalmanVelocity mv(js.at("velocity").at("r"), js.at("velocity").at("q11"), js.at("velocity").at("q22"), js.at("velocity").at("q33"), js.at("velocity").at("q44"), js.at("velocity").at("q55"), js.at("velocity").at("q66"));
        MPU mpu(b.at("address"), mv, ma);
        cfg.sensors = SensorsStruct(sc, mpu, gps, bmp, dec);
    }
    
    void parse_control_obj(const nlohmann::json &j, Config &cfg) 
    {
        int max_p, max_y, max_r, min_esc, max_esc, idle, lf, rf, lb, rb, kaw, max_diff;
        bool calib;
        json c, val, ki, kd, kp;
        std::string pos;
        c = j.at("controls");
        max_p = c.at("max_pitch_angle");
        max_r = c.at("max_roll_angle");
        max_y = c.at("max_yawn_velocity");
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
        max_diff = c.at("max_diff");
        kaw = c.at("k_aw");
        PIDControl rc(kp.at("roll"), kd.at("roll"), ki.at("roll"), kaw);
        PIDControl pc(kp.at("pitch"), kd.at("pitch"), ki.at("pitch"), kaw);
        PIDControl yc(kp.at("yawn"), kd.at("yawn"), ki.at("yawn"), kaw);
        Escs escs(rc, pc, yc, min_esc, max_esc, idle, lf, rf, lb, rb, calib, max_diff);
        cfg.controls = ControlsStruct(escs, max_p, max_r, max_y);
    }
}