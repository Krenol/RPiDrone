#include "json_config_parser.hpp"

namespace drone
{
    // const float DEGREE_UPPER_BOUND = 360.0f, OFFSET_UPPER_BOUND = 1.0f, DEGREE_LOWER_BOUND = -360.0f, OFFSET_LOWER_BOUND = 0.0f, ROTATION_UPPER_BOUND = 100.0f, ROTATION_LOWER_BOUND = -100.0f;
    // const int THROTTLE_UPPER_BOUND = 100, THROTTLE_LOWER_BOUND = 0;
    
    void from_json(const nlohmann::json &j, Config &cfg) 
    {
        cfg.leds = Leds(j.at("leds").at("on_led"), j.at("leds").at("status_led"));
        cfg.logic = Logic(j.at("logic").at("motors_off_on_disconnect"));
        cfg.server = Server(j.at("server").at("port"), j.at("server").at("bytes"), j.at("server").at("delimiter"));
        cfg.queues = Queues(j.at("queues").at("read_size"), j.at("queues").at("write_size"));
        cfg.flightcontroller = Flightcontroller(j.at("flightcontroller").at("port"), j.at("flightcontroller").at("baudrate"));
        parse_sensor_obj(j, cfg);
        parse_control_obj(j, cfg);
    }
    
    
    void parse_sensor_obj(const nlohmann::json &j, Config &cfg) 
    {
        auto js = j.at("sensors");
        int dec = js.at("decimal_places");
        int calib = js.at("calibration");
        GPS gps(js.at("gps").at("port"), js.at("gps").at("baudrate"));
        auto b = js.at("bmp");
        BMP bmp(b.at("address"), b.at("accuracy"));
        b = js.at("mpu");
        MPU mpu(b.at("address"));
        cfg.sensors = SensorsStruct(calib, mpu, gps, bmp, dec);
    }
    
    void parse_control_obj(const nlohmann::json &j, Config &cfg) 
    {
        int min_esc, max_esc, idle, lf, rf, lb, rb;
        float max_p, max_y, max_r;
        double m;
        bool calib;
        json c, val, ki, kd, kp, kaw, diffs;
        std::string pos;
        c = j.at("controls");
        max_p = c.at("max_pitch_rate");
        max_r = c.at("max_roll_rate");
        max_y = c.at("max_yaw_velocity");
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
        diffs = c.at("max_diff");
        kaw = c.at("k_aw");
        m = diffs.at("roll_rate");
        PIDControl rac(kp.at("roll_rate"), kd.at("roll_rate"), ki.at("roll_rate"), kaw.at("roll_rate"), -m, m);
        m = diffs.at("pitch_rate");
        PIDControl pac(kp.at("pitch_rate"), kd.at("pitch_rate"), ki.at("pitch_rate"), kaw.at("pitch_rate"), -m, m);
        m = diffs.at("roll_output");
        PIDControl rrc(kp.at("roll_output"), kd.at("roll_output"), ki.at("roll_output"), kaw.at("roll_output"), -m, m);
        m = diffs.at("pitch_output");
        PIDControl prc(kp.at("pitch_output"), kd.at("pitch_output"), ki.at("pitch_output"), kaw.at("pitch_output"), -m, m);
        m = diffs.at("yaw_output");
        PIDControl yrc(kp.at("yaw_output"), kd.at("yaw_output"), ki.at("yaw_output"), kaw.at("yaw_output"), -m, m);
        Escs escs(rac, pac, rrc, prc, yrc, min_esc, max_esc, idle, lf, rf, lb, rb, calib);
        cfg.controls = ControlsStruct(escs, max_p, max_r, max_y);
    }
}