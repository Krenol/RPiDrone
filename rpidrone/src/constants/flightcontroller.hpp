#ifndef DRONE_CONSTANTS_FC_H
#define DRONE_CONSTANTS_FC_H

namespace drone {
    namespace constants {
        namespace flightcontroller {
            static constexpr int OUT_MSG_SIZE = 90, SETUP_TIMEOUT_IN_SECONDS = 30;
            static constexpr char CONTROL_TOKEN[] = "<S>", ACK_TOKEN[] = "<A>", EOL = '\n', INPUT_TOKEN[] = "<S>", 
                ITEM_DELIMITER[] = ";", CONFIG_TOKEN[] = "<C>", CATEGORY_DELIMITER[] = "&", VALUE_DELIMITER[] = ",";
        }
    }
}

#endif