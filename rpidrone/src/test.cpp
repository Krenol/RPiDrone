#include "rpicomponents/rpicomponents.hpp"



#include <thread>
#include <cassert>
#include <iostream>
#include <mutex>

std::mutex mtx;

void On(rpicomponents::Led* led, int no){
	std::lock_guard<std::mutex> lck(mtx);

    for(auto i= 0; i <  no; i++) {
        auto on = led->IsOn();
		std::cout << "thread " << no << " led is " << on<< std::endl;
        on ? led->TurnOff() : led->TurnOn();
    }

    return;
}


int main() {
    rpicomponents::Pcf8574 pcf(0x48);
    rpicomponents::Pcf8591 pcf1(0x49, 124);
    //rpicomponents::Mpu6050 mpu(0x51);
    rpicomponents::Transistor tr(1, rpicomponents::NPN, rpicomponents::pin::PWM_MODE);

    //cout << btn->IsPressed()<<endl;
	//auto pin = rpicomponents::pin::PinFactory::CreatePin(12);
    rpicomponents::UltrasonicSensor uss (2, 3);

	rpicomponents::Button btn(1);

  
	std::cout << "500 mm are " << uss.UnitConverter(500, rpicomponents::UNIT_MM, rpicomponents::UNIT_M) << " m\n";
    std::cout << pcf.ToString() << std::endl;


	std::cin.get();

}

