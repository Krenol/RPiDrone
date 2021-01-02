#include <nlohmann/json.hpp>
#include <string>
#include "rpicomponents/rpicomponents.hpp"

#ifndef DRONE_JSON_PARSER_H
#define DRONE_JSON_PARSER_H

using json = nlohmann::json;

namespace drone
{
    struct Server
    {
        int port = 8889, bytes = 256;
        std::string delimiter = "\r\n";

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
        int on_led = 4, status_led = 5;

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
        int read_size = 100, write_size = 100;

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
        bool calibrate = false;
        int measurements = 1000;

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
        std::string port = "/dev/serial0";
        int baudrate = 9600;

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
        double c1 = 1, c2 = 0, r = 0.05, q11 = 0.1, q12 = 0, q21 = 0, q22 = 0.1;

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
        double q11 = 0.1, q22 = 0.1, q33 = 0.1, q44 = 0.1, q55 = 0.1, q66 = 0.1, r = 0.05;

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
    struct GpsCoordinates {
        float altitude = 0.0f, longitude = 0.0f, latitude = 0.0f;

        GpsCoordinates() {

        }

        GpsCoordinates(float altitude, float longitude, float latitude) {
            this->altitude = altitude;
            this->longitude = longitude;
            this->latitude = latitude;
        }

        GpsCoordinates(const GpsCoordinates& g) {
            this->altitude = g.altitude;
            this->longitude = g.longitude;
            this->latitude = g.latitude;
        }
    };
            this->q55 = m.q55;
            this->q66 = m.q66;
        }
    };

    struct MPU
    {
        int address = 104;
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
        double c1 = 1.0, c2 = 0.0, q11 = 0.001, q12 = 0, q21 = 0, q22 = 0.001;

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
        int address = 119, accuracy = 1;
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
        double k_p = 1.0, k_d = 1.0, k_i = 0.0, k_aw = 0.0;

        PIDControl() {

        }

        PIDControl(double k_p, double k_d, double k_i, double k_aw) {
            this->k_aw = k_aw;
            this->k_d = k_d;
            this->k_i = k_i;
            this->k_p = k_p;            
        }

        PIDControl(const PIDControl& p) {
            this->k_aw = p.k_aw;
            this->k_d = p.k_d;
            this->k_i = p.k_i;
            this->k_p = p.k_p;
        }
    };

    struct Escs
    {
        bool calibrate = false;
        int min = 700, max = 2000, idle = 800, pin_lf = 0, pin_rf = 1, pin_lb = 2, pin_rb = 3, max_diff = 50;
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
        float max_pitch = 20.0f, max_roll = 20.0f, max_yawn = 20.0f;
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
        int decimals = 1,  control_measurements = 3;
        MPU mpu;
        GPS gps;
        BMP bmp;

        SensorsStruct() {

        }

        SensorsStruct(const SensorCalibration& sc, const MPU& m, const GPS& g, const BMP& b, int decimals, int control_measurements) : sensor_calibration(sc), mpu(m), gps(g), bmp(b) {
            this->decimals = decimals;
            this->control_measurements = control_measurements;
        }

        SensorsStruct(const SensorsStruct& s) : sensor_calibration(s.sensor_calibration), mpu(s.mpu), gps(s.gps), bmp(s.bmp) {
            this->decimals = s.decimals;
            this->control_measurements = s.control_measurements;
        }
    };

    struct Logic
    {
        int main_sleep = 5000, sleep_disconnect = 200000;
        bool motors_off_disconnect = false;

        Logic() {

        }

        Logic(int main_sleep, bool motors_off_disconnect, int sleep_disconnect) {
            this->main_sleep = main_sleep;
            this->motors_off_disconnect = motors_off_disconnect;
            this->sleep_disconnect = sleep_disconnect;
        }

        Logic(const Logic& l) {
            this->main_sleep = l.main_sleep;
            this->motors_off_disconnect = l.motors_off_disconnect;
            this->sleep_disconnect = l.sleep_disconnect;
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

    struct Joystick {
        float degrees = 0, offset = 0, rotation = 0;

        Joystick() {

        }

        Joystick(float degrees, float offset, float rotation) {
            this->degrees = degrees;
            this->offset = offset;
            this->rotation = rotation;
        }

        Joystick(const Joystick& j) {
            this->degrees = j.degrees;
            this->offset = j.offset;
            this->rotation = j.rotation;
        }
    };


    struct Input {
        int throttle = 0;
        Joystick joystick;
        rpicomponents::GPSCoordinates gps;

        Input() {

        }

        Input(const Joystick& j, const rpicomponents::GPSCoordinates& gc, int throttle) : joystick(j), gps(gc)
        {
            this->throttle = throttle;
        }

        Input(const Input& i) : joystick(i.joystick), gps(i.gps)
        {
            this->throttle = i.throttle;
        }
    };

    void from_json(const nlohmann::json &j, Config &cfg);

    void from_json(const nlohmann::json &j, Input &i);

    void parse_sensor_obj(const nlohmann::json &j, Config &cfg);

    void parse_control_obj(const nlohmann::json &j, Config &cfg);

    static const float DEGREE_UPPER_BOUND = 360.0f, OFFSET_UPPER_BOUND = 1.0f, DEGREE_LOWER_BOUND = -360.0f, OFFSET_LOWER_BOUND = 0.0f, ROTATION_UPPER_BOUND = 100.0f, ROTATION_LOWER_BOUND = -100.0f;
    static const int THROTTLE_UPPER_BOUND = 100, THROTTLE_LOWER_BOUND = 0;

} // namespace drone

#endif