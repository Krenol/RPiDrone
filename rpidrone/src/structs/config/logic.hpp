#ifndef DRONE_STRUCT_CONFIG_LOGIC_H
#define DRONE_STRUCT_CONFIG_LOGIC_H

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct Logic
            {
                bool motors_off_disconnect = false;
                int resetPin = -1;

                Logic()
                {
                }

                Logic(bool motors_off_disconnect, int resetPin)
                {
                    this->motors_off_disconnect = motors_off_disconnect;
                    this->resetPin = resetPin;
                }

                Logic(const Logic &l)
                {
                    this->motors_off_disconnect = l.motors_off_disconnect;
                    this->resetPin = l.resetPin;
                }
            };
        }
    }
}
#endif