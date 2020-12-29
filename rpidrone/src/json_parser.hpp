#include <nlohmann/json.hpp>
#include <string>

#ifndef DRONE_JSON_PARSER_H
#define DRONE_JSON_PARSER_H

using json = nlohmann::json;

namespace drone
{
    struct Config
    {
        Logic logic;
        Server server;
        Leds leds;
        Queues queues;
        Sensors sensors;
        Controls controls;

        Config(const Logic& logic, const Server& server, const Leds& leds, const Queues& queues, const Sensors& sensors, const Controls& controls) : logic(logic), server(server), leds(leds), queues(queues), sensors(sensors), controls(controls) {

        }

        Config(const Config& c) : logic(c.logic), server(c.server), leds(c.leds), queues(c.queues), sensors(c.sensors), controls(c.controls) {
            
        }
    };

    struct Server
    {
        int port, bytes;
        std::string delimiter;
        Server(int port, int bytes, const std::string& delimiter){
            this->port = port;
            this->bytes = bytes;
            this->delimiter = delimiter;
        }

        Server(const Server& s){
            this->port = s.port;
            this->bytes = s.bytes;
            this->delimiter = s.delimiter;
        }
    };

    struct Leds
    {
        int on_led, status_led;
        Leds(int on_led, int status_led) {
            this->on_led = on_led;
            this->status_led = status_led;
        }

        Leds(const Leds& l) {
            this->on_led = l.on_led;
            this->status_led = l.status_led;
        }
    };

    struct Queues
    {
        int read_size, write_size;
        Queues(int read_size, int write_size) {
            this->read_size = read_size;
            this->write_size = write_size;
        }

        Queues(const Queues& q) {
            this->read_size = q.read_size;
            this->write_size = q.write_size;
        }
    };

    struct Sensors
    {
        SensorCalibration sensor_calibration;
        int decimals;
        MPU mpu;
        GPS gps;
        BMP bmp;

        Sensors(const SensorCalibration& sc, const MPU& m, const GPS& g, const BMP& b, int decimals) : sensor_calibration(sc), mpu(m), gps(g), bmp(b) {
            this->decimals = decimals;
        }

        Sensors(const Sensors& s) : sensor_calibration(s.sensor_calibration), mpu(s.mpu), gps(s.gps), bmp(s.bmp) {
            this->decimals = s.decimals;
        }
    };
    struct SensorCalibration
    {
        bool calibrate;
        int measurements;
        SensorCalibration(bool calibrate, int measurements) {
            this->calibrate = calibrate;
            this->measurements = measurements;
        }

        SensorCalibration(const SensorCalibration& s) {
            this->calibrate = s.calibrate;
            this->measurements = s.measurements;
        }
    };

    struct GPS
    {
        std::string port;
        int baudrate;
        GPS(const std::string& port, int baudrate) {
            this->port = port;
            this->baudrate = baudrate;
        }

        GPS(const GPS& g) {
            this->port = g.port;
            this->baudrate = g.baudrate;
        }
    };

    struct MPU
    {
        int address;
        MPUKalmanVelocity kalman_velocity;
        MPUKalmanAngles kalman_angles;
        MPU(int address, const MPUKalmanVelocity& kv, const MPUKalmanAngles& ka) : kalman_angles(ka), kalman_velocity(kv) {
            this->address = address;
        }

        MPU(const MPU& m) : kalman_angles(m.kalman_angles), kalman_velocity(m.kalman_velocity) {
            this->address = m.address;
        }
    };

    struct MPUKalmanVelocity
    {
        double c1, c2, r, q11, q12, q21, q22;
        MPUKalmanVelocity(double c1, double c2, double r, double q11, double q12, double q21, double q22) {
            this->c1 = c1;
            this->c2 = c2;
            this->q11 = q11;
            this->q12 = q12;
            this->q21 = q21;
            this->q22 = q22;
            this->r = r;
        }

        MPUKalmanVelocity(const MPUKalmanVelocity& m) {
            this->c1 = m.c1;
            this->c2 = m.c2;
            this->q11 = m.q11;
            this->q12 = m.q12;
            this->q21 = m.q21;
            this->q22 = m.q22;
            this->r = m.r;
        }
    };

    struct MPUKalmanAngles
    {
        double q11, q22, q33, q44, q55, q66, r;
        MPUKalmanAngles(double r, double q11, double q22, double q33, double q44, double q55, double q66) {
            this->r = r;
            this->q11 = q11;
            this->q22 = q22;
            this->q33 = q33;
            this->q44 = q44;
            this->q55 = q55;
            this->q66 = q66;
        }

        MPUKalmanAngles(const MPUKalmanAngles& m) {
            this->r = m.r;
            this->q11 = m.q11;
            this->q22 = m.q22;
            this->q33 = m.q33;
            this->q44 = m.q44;
            this->q55 = m.q55;
            this->q66 = m.q66;
        }
    };

    struct BMP
    {
        int address, accuracy;
        BMPKalman kalman;
        BMP(int address, int accuracy, const BMPKalman& k) : kalman{k} {
            this->accuracy = accuracy;
            this->address = address;
        }

        BMP(const BMP& b) : kalman{b.kalman} {
            this->accuracy = b.accuracy;
            this->address = b.address;
        }
    };

    struct BMPKalman
    {
        double c1, c2, q11, q12, q21, q22;

        BMPKalman(double c1, double c2, double q11, double q12, double q21, double q22) {
            this->c1 = c1;
            this->c2 = c2;
            this->q11 = q11;
            this->q12 = q12;
            this->q21 = q21;
            this->q22 = q22;
        }

        BMPKalman(const BMPKalman& k) {
            this->c1 = k.c1;
            this->c2 = k.c2;
            this->q11 = k.q11;
            this->q12 = k.q12;
            this->q21 = k.q21;
            this->q22 = k.q22;
        }
    };

    struct Controls
    {
        double max_pitch, max_roll, max_yawn;
        Escs escs;

        Controls(const Escs& esc, double max_pitch, double max_roll, double max_yawn) : escs(esc) {
            this->max_pitch = max_pitch;
            this->max_roll = max_roll;
            this->max_yawn = max_yawn;
        }

        Controls(const Controls& c) : escs(c.escs) {
            this->max_pitch = c.max_pitch;
            this->max_roll = c.max_roll;
            this->max_yawn = c.max_yawn;
        }
    };

    struct Escs
    {
        bool calibrate;
        int min, max, idle, pin_lf, pin_rf, pin_lb, pin_rb;
        PIDControl roll, pitch, yawn;

        Escs(const PIDControl& roll, const PIDControl& pitch, const PIDControl& yawn, int min, int max, int idle, int pin_lf, int pin_rf, int pin_lb, int pin_rb, bool calibrate) : roll(roll), pitch(pitch), yawn(yawn)  {
            this->calibrate = calibrate;
            this->idle = idle;
            this->max = max;
            this->min = min;
            this->pin_lb = pin_lb;
            this->pin_rb = pin_rb;
            this->pin_lf = pin_lf;
            this->pin_rf = pin_rf;
        }

        Escs(const Escs& e) : roll(e.roll), pitch(e.pitch), yawn(e.yawn) {
            this->calibrate = e.calibrate;
            this->idle = e.idle;
            this->max = e.max;
            this->min = e.min;
            this->pin_lb = e.pin_lb;
            this->pin_rb = e.pin_rb;
            this->pin_lf = e.pin_lf;
            this->pin_rf = e.pin_rf;
        }
    };

    struct PIDControl
    {
        double k_p, k_d, k_i, max_diff, k_aw;
        PIDControl(double k_p, double k_d, double k_i, double max_diff, double k_aw) {
            this->k_aw = k_aw;
            this->k_d = k_d;
            this->k_i = k_i;
            this->max_diff = max_diff;
        }

        PIDControl(const PIDControl& p) {
            this->k_aw = p.k_aw;
            this->k_d = p.k_d;
            this->k_i = p.k_i;
            this->max_diff = p.max_diff;
        }
    };

    struct Logic
    {
        int main_sleep;
        bool motors_off_disconnect;

        Logic(int main_sleep, bool motors_off_disconnect) {
            this->main_sleep = main_sleep;
            this->motors_off_disconnect = motors_off_disconnect;
        }

        Logic(const Logic& l) {
            this->main_sleep = l.main_sleep;
            this->motors_off_disconnect = l.motors_off_disconnect;
        }
    };

    void to_json(nlohmann::json &j, const Config &cfg);

    void from_json(const nlohmann::json &j, Config &cfg);
} // namespace drone

#endif