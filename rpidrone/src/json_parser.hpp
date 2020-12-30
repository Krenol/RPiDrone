#include <nlohmann/json.hpp>
#include <string>

#ifndef DRONE_JSON_PARSER_H
#define DRONE_JSON_PARSER_H

using json = nlohmann::json;

namespace drone
{
    struct Server
    {
        int port, bytes;
        std::string delimiter;

        Server() {

        }

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

        Leds() {

        }

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

        Queues() {

        }

        Queues(int read_size, int write_size) {
            this->read_size = read_size;
            this->write_size = write_size;
        }

        Queues(const Queues& q) {
            this->read_size = q.read_size;
            this->write_size = q.write_size;
        }
    };

    struct SensorCalibration
    {
        bool calibrate;
        int measurements;

        SensorCalibration() {

        }

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

        GPS() {

        }

        GPS(const std::string& port, int baudrate) {
            this->port = port;
            this->baudrate = baudrate;
        }

        GPS(const GPS& g) {
            this->port = g.port;
            this->baudrate = g.baudrate;
        }
    };

    struct MPUKalmanAngles
    {
        double c1, c2, r, q11, q12, q21, q22;

        MPUKalmanAngles() {

        }

        MPUKalmanAngles(double c1, double c2, double r, double q11, double q12, double q21, double q22) {
            this->c1 = c1;
            this->c2 = c2;
            this->q11 = q11;
            this->q12 = q12;
            this->q21 = q21;
            this->q22 = q22;
            this->r = r;
        }

        MPUKalmanAngles(const MPUKalmanAngles& m) {
            this->c1 = m.c1;
            this->c2 = m.c2;
            this->q11 = m.q11;
            this->q12 = m.q12;
            this->q21 = m.q21;
            this->q22 = m.q22;
            this->r = m.r;
        }
    };

    struct MPUKalmanVelocity
    {
        double q11, q22, q33, q44, q55, q66, r;

        MPUKalmanVelocity() {

        }

        MPUKalmanVelocity(double r, double q11, double q22, double q33, double q44, double q55, double q66) {
            this->r = r;
            this->q11 = q11;
            this->q22 = q22;
            this->q33 = q33;
            this->q44 = q44;
            this->q55 = q55;
            this->q66 = q66;
        }

        MPUKalmanVelocity(const MPUKalmanVelocity& m) {
            this->r = m.r;
            this->q11 = m.q11;
            this->q22 = m.q22;
            this->q33 = m.q33;
            this->q44 = m.q44;
            this->q55 = m.q55;
            this->q66 = m.q66;
        }
    };

    struct MPU
    {
        int address;
        MPUKalmanVelocity kalman_velocity;
        MPUKalmanAngles kalman_angles;

        MPU() {

        }

        MPU(int address, const MPUKalmanVelocity& kv, const MPUKalmanAngles& ka) : kalman_angles(ka), kalman_velocity(kv) {
            this->address = address;
        }

        MPU(const MPU& m) : kalman_angles(m.kalman_angles), kalman_velocity(m.kalman_velocity) {
            this->address = m.address;
        }
    };

    struct BMPKalman
    {
        double c1, c2, q11, q12, q21, q22;

        BMPKalman() {

        }

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

    struct BMP
    {
        int address, accuracy;
        BMPKalman kalman;

        BMP() {

        }

        BMP(int address, int accuracy, const BMPKalman& k) : kalman{k} {
            this->accuracy = accuracy;
            this->address = address;
        }

        BMP(const BMP& b) : kalman{b.kalman} {
            this->accuracy = b.accuracy;
            this->address = b.address;
        }
    };

    struct PIDControl
    {
        double k_p, k_d, k_i, k_aw;

        PIDControl() {

        }

        PIDControl(double k_p, double k_d, double k_i, double k_aw) {
            this->k_aw = k_aw;
            this->k_d = k_d;
            this->k_i = k_i;
            
        }

        PIDControl(const PIDControl& p) {
            this->k_aw = p.k_aw;
            this->k_d = p.k_d;
            this->k_i = p.k_i;
        }
    };

    struct Escs
    {
        bool calibrate;
        int min, max, idle, pin_lf, pin_rf, pin_lb, pin_rb, max_diff;
        PIDControl roll, pitch, yawn;

        Escs() {

        }

        Escs(const PIDControl& roll, const PIDControl& pitch, const PIDControl& yawn, int min, int max, int idle, int pin_lf, int pin_rf, int pin_lb, int pin_rb, bool calibrate, double max_diff) : roll(roll), pitch(pitch), yawn(yawn)  {
            this->calibrate = calibrate;
            this->idle = idle;
            this->max = max;
            this->min = min;
            this->pin_lb = pin_lb;
            this->pin_rb = pin_rb;
            this->pin_lf = pin_lf;
            this->pin_rf = pin_rf;
            this->max_diff = max_diff;
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
            this->max_diff = e.max_diff;
        }
    };

    struct ControlsStruct
    {
        float max_pitch, max_roll, max_yawn;
        Escs escs;

        ControlsStruct() {

        }

        ControlsStruct(const Escs& esc, float max_pitch, float max_roll, float max_yawn) : escs(esc) {
            this->max_pitch = max_pitch;
            this->max_roll = max_roll;
            this->max_yawn = max_yawn;
        }

        ControlsStruct(const ControlsStruct& c) : escs(c.escs) {
            this->max_pitch = c.max_pitch;
            this->max_roll = c.max_roll;
            this->max_yawn = c.max_yawn;
        }
    };

    struct SensorsStruct
    {
        SensorCalibration sensor_calibration;
        int decimals;
        MPU mpu;
        GPS gps;
        BMP bmp;

        SensorsStruct() {

        }

        SensorsStruct(const SensorCalibration& sc, const MPU& m, const GPS& g, const BMP& b, int decimals) : sensor_calibration(sc), mpu(m), gps(g), bmp(b) {
            this->decimals = decimals;
        }

        SensorsStruct(const SensorsStruct& s) : sensor_calibration(s.sensor_calibration), mpu(s.mpu), gps(s.gps), bmp(s.bmp) {
            this->decimals = s.decimals;
        }
    };

    struct Logic
    {
        int main_sleep;
        bool motors_off_disconnect;

        Logic() {

        }

        Logic(int main_sleep, bool motors_off_disconnect) {
            this->main_sleep = main_sleep;
            this->motors_off_disconnect = motors_off_disconnect;
        }

        Logic(const Logic& l) {
            this->main_sleep = l.main_sleep;
            this->motors_off_disconnect = l.motors_off_disconnect;
        }
    };

    struct Config
    {
        Logic logic;
        Server server;
        Leds leds;
        Queues queues;
        SensorsStruct sensors;
        ControlsStruct controls;

        Config(){

        }

        Config(const Logic& logic, const Server& server, const Leds& leds, const Queues& queues, const SensorsStruct& sensors, const ControlsStruct& controls) : logic(logic), server(server), leds(leds), queues(queues), sensors(sensors), controls(controls) {

        }

        Config(const Config& c) : logic(c.logic), server(c.server), leds(c.leds), queues(c.queues), sensors(c.sensors), controls(c.controls) {
            
        }
    };
    //{"joystick": {"degress": 0, "offset": 0, "rotation":0}, "throttle": 12, "gps":{"altitude": 0, "longitude": 0, "latitude": 0}}
    void to_json(nlohmann::json &j, const Config &cfg);

    void from_json(const nlohmann::json &j, Config &cfg);

    void parse_sensor_obj(const nlohmann::json &j, Config &cfg);

    void parse_control_obj(const nlohmann::json &j, Config &cfg);

} // namespace drone

#endif