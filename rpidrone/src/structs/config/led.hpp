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
                int on_led_pin = 4, status_led_pin = 5;

                Leds()
                {
                }

                Leds(int on_led_pin, int status_led_pin)
                {
                    this->on_led_pin = on_led_pin;
                    this->status_led_pin = status_led_pin;
                }

                Leds(const Leds &l)
                {
                    this->on_led_pin = l.on_led_pin;
                    this->status_led_pin = l.status_led_pin;
                }
            };
        }
    }
}

#endif