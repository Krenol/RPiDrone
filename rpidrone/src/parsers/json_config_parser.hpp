#ifndef DRONE_JSON_CONFIG_PARSER_H
#define DRONE_JSON_CONFIG_PARSER_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace drone
{

    struct Server
    {
        int port = 8889, bytes = 256;
        std::string delimiter = "\r\n";

        Server()
        {
        }

        Server(int port, int bytes, const std::string &delimiter)
        {
            this->port = port;
            this->bytes = bytes;
            this->delimiter = delimiter;
        }

        Server(const Server &s)
        {
            this->port = s.port;
            this->bytes = s.bytes;
            this->delimiter = s.delimiter;
        }
    };

    struct Leds
    {
        int on_led = 4, status_led = 5;

        Leds()
        {
        }

        Leds(int on_led, int status_led)
        {
            this->on_led = on_led;
            this->status_led = status_led;
        }

        Leds(const Leds &l)
        {
            this->on_led = l.on_led;
            this->status_led = l.status_led;
        }
    };

    struct Queues
    {
        int read_size = 100, write_size = 100;

        Queues()
        {
        }

        Queues(int read_size, int write_size)
        {
            this->read_size = read_size;
            this->write_size = write_size;
        }

        Queues(const Queues &q)
        {
            this->read_size = q.read_size;
            this->write_size = q.write_size;
        }
    };

    struct GPS
    {
        std::string port = "/dev/serial0";
        int baudrate = 9600;

        GPS()
        {
        }

        GPS(const std::string &port, int baudrate)
        {
            this->port = port;
            this->baudrate = baudrate;
        }

        GPS(const GPS &g)
        {
            this->port = g.port;
            this->baudrate = g.baudrate;
        }
    };

    struct MPU
    {
        int address = 104;

        MPU()
        {
        }

        MPU(int address)
        {
            this->address = address;
        }

        MPU(const MPU &m)
        {
            this->address = m.address;
        }
    };

    struct BMP
    {
        int address = 119, accuracy = 1;

        BMP()
        {
        }

        BMP(int address, int accuracy)
        {
            this->accuracy = accuracy;
            this->address = address;
        }

        BMP(const BMP &b)
        {
            this->accuracy = b.accuracy;
            this->address = b.address;
        }
    };

    struct PIDControl
    {
        double k_p = 1.0, k_d = 1.0, k_i = 0.0, k_aw = 0.0, max = 0.0, min = 0.0;

        PIDControl()
        {
        }

        PIDControl(double k_p, double k_d, double k_i, double k_aw, double min, double max)
        {
            this->k_aw = k_aw;
            this->k_d = k_d;
            this->k_i = k_i;
            this->k_p = k_p;
            this->min = min;
            this->max = max;
        }

        PIDControl(const PIDControl &p)
        {
            this->k_aw = p.k_aw;
            this->k_d = p.k_d;
            this->k_i = p.k_i;
            this->k_p = p.k_p;
            this->min = p.min;
            this->max = p.max;
        }
    };

    struct Escs
    {
        bool calibrate = false;
        int min = 700, max = 2000, idle = 800, pin_lf = 0, pin_rf = 1, pin_lb = 2, pin_rb = 3;
        PIDControl roll_rate, pitch_rate, roll_output, pitch_output, yaw_output;

        Escs()
        {
        }

        Escs(const PIDControl &roll_rate, const PIDControl &pitch_rate, const PIDControl &roll_output, const PIDControl &pitch_output, const PIDControl &yaw_output, int min, int max, int idle, int pin_lf, int pin_rf, int pin_lb, int pin_rb, bool calibrate) : roll_rate(roll_rate), pitch_rate(pitch_rate), roll_output(roll_output), pitch_output(pitch_output), yaw_output(yaw_output)
        {
            this->calibrate = calibrate;
            this->idle = idle;
            this->max = max;
            this->min = min;
            this->pin_lb = pin_lb;
            this->pin_rb = pin_rb;
            this->pin_lf = pin_lf;
            this->pin_rf = pin_rf;
        }

        Escs(const Escs &e) : roll_rate(e.roll_rate), pitch_rate(e.pitch_rate), roll_output(e.roll_output), pitch_output(e.pitch_output), yaw_output(e.yaw_output)
        {
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

    struct ControlsStruct
    {
        float max_pitch = 20.0f, max_roll = 20.0f, max_yawn = 20.0f;
        Escs escs;

        ControlsStruct()
        {
        }

        ControlsStruct(const Escs &esc, float max_pitch, float max_roll, float max_yawn) : escs(esc)
        {
            this->max_pitch = max_pitch;
            this->max_roll = max_roll;
            this->max_yawn = max_yawn;
        }

        ControlsStruct(const ControlsStruct &c) : escs(c.escs)
        {
            this->max_pitch = c.max_pitch;
            this->max_roll = c.max_roll;
            this->max_yawn = c.max_yawn;
        }
    };

    struct SensorsStruct
    {
        bool calibrate;
        int decimals = 1;
        MPU mpu;
        GPS gps;
        BMP bmp;

        SensorsStruct()
        {
        }

        SensorsStruct(bool calibrate, const MPU &m, const GPS &g, const BMP &b, int decimals) : mpu(m), gps(g), bmp(b)
        {
            this->decimals = decimals;
            this->calibrate = calibrate;
        }

        SensorsStruct(const SensorsStruct &s) : mpu(s.mpu), gps(s.gps), bmp(s.bmp)
        {
            this->decimals = s.decimals;
            this->calibrate = s.calibrate;
        }
    };

    struct Logic
    {
        bool motors_off_disconnect = false;

        Logic()
        {
        }

        Logic(bool motors_off_disconnect)
        {
            this->motors_off_disconnect = motors_off_disconnect;
        }

        Logic(const Logic &l)
        {
            this->motors_off_disconnect = l.motors_off_disconnect;
        }
    };

    struct Flightcontroller {
        std::string port = "/dev/ttyACM0";
        int baudrate = 115200;

        Flightcontroller() {

        }

        Flightcontroller(const std::string &port, int baudrate) 
        {
            this->port = port;
            this->baudrate = baudrate;
        }

        Flightcontroller(const Flightcontroller &fc) 
        {
            this->port = fc.port;
            this->baudrate = fc.baudrate;
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
        Flightcontroller flightcontroller;

        Config()
        {
        }

        Config(const Logic &logic, const Server &server, const Leds &leds, const Queues &queues, const SensorsStruct &sensors, const ControlsStruct &controls, const Flightcontroller &flightcontroller) : logic(logic), server(server), leds(leds), queues(queues), sensors(sensors), controls(controls), flightcontroller(flightcontroller)
        {
        }

        Config(const Config &c) : logic(c.logic), server(c.server), leds(c.leds), queues(c.queues), sensors(c.sensors), controls(c.controls), flightcontroller(c.flightcontroller)
        {
        }
    };

    void from_json(const nlohmann::json &j, Config &cfg);

    void parse_sensor_obj(const nlohmann::json &j, Config &cfg);

    void parse_control_obj(const nlohmann::json &j, Config &cfg);

} // namespace drone

#endif