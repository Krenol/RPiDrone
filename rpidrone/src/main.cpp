#include "loop.hpp"

int main() {
    drone::Loop l ("/home/pi/mnt/RPiDrone/config.json");

    l.startupDrone();
    while(1) {
        l.loop();
    }
    return 0;
}

