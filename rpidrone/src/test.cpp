#include "rpicomponents/rpicomponents.hpp"
#include <iostream>



int main() {
    auto pin = rpicomponents::pin::PinCreator::CreatePin(rpicomponents::pin::GPIO17);
    rpicomponents::Led led(pin);

    led.TurnOn();

    rpicomponents::utils::Waiter::SleepSecs(2);
    led.TurnOff();
     

	std::cin.get();

}

