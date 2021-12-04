#ifndef DRONE_STRUCT_CONFIG_PID_H
#define DRONE_STRUCT_CONFIG_PID_H

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct PID
            {
                double k_p = 1.0, k_d = 1.0, k_i = 0.0, k_aw = 0.0, max = 0.0, min = 0.0;

                PID()
                {
                }

                PID(double k_p, double k_d, double k_i, double k_aw, double min, double max)
                {
                    this->k_aw = k_aw;
                    this->k_d = k_d;
                    this->k_i = k_i;
                    this->k_p = k_p;
                    this->min = min;
                    this->max = max;
                }

                PID(const PID &p)
                {
                    this->k_aw = p.k_aw;
                    this->k_d = p.k_d;
                    this->k_i = p.k_i;
                    this->k_p = p.k_p;
                    this->min = p.min;
                    this->max = p.max;
                }
            };
        }
    }
}
#endif