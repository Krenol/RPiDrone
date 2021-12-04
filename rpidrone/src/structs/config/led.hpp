#ifndef DRONE_STRUCT_CONFIG_LED_H
#define DRONE_STRUCT_CONFIG_LED_H

namespace drone
{
    namespace structs
    {
        namespace config
        {
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
        }
    }
}

#endif