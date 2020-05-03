#include "rpicomponents/rpicomponents.hpp"
#include <iostream>



int main() {
    rpicomponents::Pcf8574 pcf(0x48);
    auto p1 = rpicomponents::pin::PinCreator::CreatePin(1);
    auto p2 = rpicomponents::pin::PinCreator::CreatePin(2);
    rpicomponents::UltrasonicSensor uss (p1, p2);

  
	std::cout << "500 mm are " << uss.UnitConverter(500, rpicomponents::UNIT_MM, rpicomponents::UNIT_M) << " m\n";
    std::cout << pcf.ToString() << std::endl;


	std::cin.get();

}

