#ifndef DRONE_WEBSOCKET_H
#define DRONE_WEBSOCKET_H

#include "../external/Simple-WebSocket-Server/server_ws.hpp"
#include <memory>
#include <string>
#include <atomic>
#include "design_patterns/design_patterns.hpp"
#include <thread>

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using WsEndpoint = SimpleWeb::SocketServerBase<SimpleWeb::WS>::Endpoint;

namespace drone
{
    class Websocket {
        private:
            WsServer server_;
            std::shared_ptr<WsServer::Connection> connection_;
            std::atomic_bool connected_ {false};
            design_patterns::Queue<std::string> queue_;
            const int q_size_;
            std::thread srv_thrd_;

            void handleIncomingMessage(const std::string& msg);

            void handleConnectionOpening(std::shared_ptr<WsServer::Connection> connection);

            void handleConnectionClosing(std::shared_ptr<WsServer::Connection> connection, int status);

            void handleEndpointErrors(std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec);

            void setupWebsocketEndpointEvents(WsEndpoint &endpoint);

        public:
            Websocket(int q_size);

            void init(const std::string &path, int port);

            bool hasConnection() const;

            void writeMessage(const std::string& msg);

            void startWebsocketThread();

            void stopWebsocketThread();

            void disconnectConnectedClient();

            void getMessage(std::string &msg);

            bool hasMessages();
    };
}

#endif //DRONE_WEBSOCKET_H