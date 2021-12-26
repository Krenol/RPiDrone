#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

#include <memory>
#include <string>
#include "structs/config/config.hpp"
#include "middleware/client.hpp"
#include "middleware/flightcontroller.hpp"


using json = nlohmann::json;

namespace drone {
    class Loop  {
        public:
            Loop();

            void loop(middleware::Client &clientMw, middleware::Flightcontroller &fcMw, const structs::config::Config &config);
            
    };
}

#endif