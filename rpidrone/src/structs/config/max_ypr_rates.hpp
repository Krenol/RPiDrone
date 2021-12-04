#ifndef DRONE_STRUCT_CONFIG_MAX_YPR_RATES_H
#define DRONE_STRUCT_CONFIG_MAX_YPR_RATES_H

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct MaxYprRates
            {
                float maxPitchRate = 20.0f, maxRollRate = 20.0f, maxYawVel = 20.0f;
                

                MaxYprRates()
                {
                }

                MaxYprRates(float maxYawVel, float maxPitchRate, float maxRollRate)
                {
                    this->maxPitchRate = maxPitchRate;
                    this->maxRollRate = maxRollRate;
                    this->maxYawVel = maxYawVel;
                }

                MaxYprRates(const MaxYprRates &c)
                {
                    maxPitchRate = c.maxPitchRate;
                    maxRollRate = c.maxRollRate;
                    maxYawVel = c.maxYawVel;
                }
            };
        }
    }
}

#endif