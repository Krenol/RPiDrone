#ifndef DRONE_STRUCT_CONFIG_ESC_PINS_H
#define DRONE_STRUCT_CONFIG_ESC_PINS_H

#include "pid.hpp"

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct EscPins
            {
                int leftFrontPin, leftBackPin, rightFrontPin, rightBackPin;

                EscPins()
                {
                }

                EscPins(int leftFrontPin, int leftBackPin, int rightFrontPin, int rightBackPin)
                {
                    this->leftBackPin = leftBackPin;
                    this->leftFrontPin = leftFrontPin;
                    this->rightBackPin = rightBackPin;
                    this->rightFrontPin = rightFrontPin;
                }

                EscPins(const EscPins &e) 
                {
                    leftBackPin = e.leftBackPin;
                    leftFrontPin = e.leftFrontPin;
                    rightBackPin = e.rightBackPin;
                    rightFrontPin = e.rightFrontPin;
                }
            };
        }
    }
}
#endif