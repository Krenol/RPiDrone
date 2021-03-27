#ifndef _ESC_H_
#define _ESC_H_

#include <Servo.h>

class ESC {
    private:
        Servo esc_;
        uint16_t max_scale_;
        bool initialized_ = false;

    public:
        ESC();

        void init(uint8_t pin, uint16_t min_pulse_w, uint16_t max_pulse_w, uint16_t max_scale, bool calibrate = false);
        
        void calibrateESC();

        void setSpeed(uint16_t speed);
};

#endif /* _ESC_H_ */

