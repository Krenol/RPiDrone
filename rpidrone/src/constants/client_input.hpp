#ifndef DRONE_CONSTANTS_CLIENT_INPUT_H
#define DRONE_CONSTANTS_CLIENT_INPUT_H

namespace drone {
    namespace constants {
        namespace clientinput {
            static constexpr float DEGREE_UPPER_BOUND = 360.0f, OFFSET_UPPER_BOUND = 1.0f, DEGREE_LOWER_BOUND = -360.0f, 
                OFFSET_LOWER_BOUND = 0.0f, ROTATION_UPPER_BOUND = 100.0f, ROTATION_LOWER_BOUND = -100.0f;
            static constexpr int THROTTLE_UPPER_BOUND = 100, THROTTLE_LOWER_BOUND = 0;
        }
    }
}

#endif