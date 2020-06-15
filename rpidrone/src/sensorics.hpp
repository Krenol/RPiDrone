#include "rpicomponents/rpicomponents.hpp"
#include <memory>

#ifndef DRONE_SENSORICS_H
#define DRONE_SENSORICS_H

namespace drone {
    struct Distances {
        float dist_left {0}, dist_right {0}, dist_top {0}, dist_bottom {0}, dist_front {0}, dist_back {0};
    };

    class Sensorics {
        public:
            Sensorics();
            Sensorics(rpicomponents::DISTANCE_UNIT unit);

            Distances getDistances() const;         
        
        private:
            const rpicomponents::DISTANCE_UNIT unit_;
            std::unique_ptr<rpicomponents::UltrasonicSensor> uss_left_, uss_right_, uss_top_, uss_bottom_, uss_front_, uss_back_;
            

    };
}

#endif