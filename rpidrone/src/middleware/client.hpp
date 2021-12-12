#ifndef DRONE_MIDDLEWARE_CLIENT_H
#define DRONE_MIDDLEWARE_CLIENT_H

#include "../websocket.hpp"
#include "../structs/middleware/client_input.hpp"
#include "../structs/middleware/client_output.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace drone {
    namespace middleware {
        class Client {
            private:
                std::unique_ptr<Websocket> websocket_;
                json inputJson_, outputJson_;
                std::string inputStringMsg_;
                void parseClientOutput(const structs::middleware::ClientOutput &outMsg);
                bool parseClientInput(const structs::middleware::ClientOutput &outMsg);
                bool parseStringMessageToJson();
            
            public:
                Client(std::unique_ptr<Websocket> websocket);

                bool sendToClient(const structs::middleware::ClientOutput &outMsg);

                bool receiveFromClient(structs::middleware::ClientInput &inMsg);

                bool clientConnectionAvailable();

                bool clientMessagesAvailable();
        };
    }
}

#endif