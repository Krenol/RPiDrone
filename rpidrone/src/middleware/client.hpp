#ifndef DRONE_MIDDLEWARE_CLIENT_H
#define DRONE_MIDDLEWARE_CLIENT_H

#include "../websocket.hpp"
#include "../structs/middleware/input.hpp"
#include "../structs/middleware/output.hpp"
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
                void parseOutputJson(const structs::middleware::Output &outMsg);
                bool parseInput(structs::middleware::Input &inMsg);
                bool parseClientInpuStringToJson();
            
            public:
                Client(std::unique_ptr<Websocket> websocket);

                bool sendToClient(const structs::middleware::Output &outMsg);

                bool receiveFromClient(structs::middleware::Input &clientInputMsg);

                bool clientConnectionAvailable();

                bool clientMessagesAvailable();

                void disconnectConnectedClient();
        };
    }
}

#endif