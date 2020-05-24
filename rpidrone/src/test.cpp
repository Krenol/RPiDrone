//#include "rpicomponents/rpicomponents.hpp"
#include <iostream>
#include <string>
#include "publisher.hpp"
#include "subscriber.hpp"
#include "queue.hpp"
#include <memory>

class Sub : public rpidrone::Subscriber<int> {
    void update(const int& msg) {
        std::cout << "rcvd msg: " << msg << std::endl;
    }
};


int main() {
    /*auto pin = rpicomponents::pin::PinCreator::CreatePin(rpicomponents::pin::GPIO17);
    rpicomponents::Led led(pin);
    
    led.TurnOn();

    rpicomponents::utils::Waiter::SleepSecs(2);
    led.TurnOff();*/

    rpidrone::Publisher<int> pub;
    Sub sub;

    pub.subscribe(std::make_shared<Sub>(sub));
    pub.notifyAll(1);     
    rpidrone::Queue<int> q;

	std::cin.get();

}

