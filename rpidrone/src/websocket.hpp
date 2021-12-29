#ifndef DRONE_WEBSOCKET_H
#define DRONE_WEBSOCKET_H

#include "../external/Simple-WebSocket-Server/server_ws.hpp"
#include <memory>
#include <string>
#include <atomic>
#include "design_patterns/design_patterns.hpp"
#include <thread>
#include <future>

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using WsEndpoint = SimpleWeb::SocketServerBase<SimpleWeb::WS>::Endpoint;

namespace drone
{
    class Websocket {
        private:
            WsServer server_;
            std::shared_ptr<WsServer::Connection> connection_;
            std::atomic_bool connected_ {false}, activeServerThread_ {false};
            design_patterns::Queue<std::string> queue_;
            const int q_size_;
            std::thread srv_thrd_;

            void onIncomingMessage(const std::string& msg);

            void onConnectionOpening(std::shared_ptr<WsServer::Connection> connection);

            void onConnectionClosing(std::shared_ptr<WsServer::Connection> connection, int status);

            void onEndpointErrors(std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec);

            void setupWebsocketEndpointEvents(WsEndpoint &endpoint);

            void onMessageSendError(const SimpleWeb::error_code &ec);

            void sendMessageToConnectedClient(const std::string &msg);

            void sendMessageToConnectedClient(const std::string &msg, std::promise<bool> &promise);

            void removeOldestQueueMessage();

        public:
            Websocket(int q_size, const std::string &context, int port);

            bool hasConnection() const;

            void writeMessage(const std::string& msg);

            void writeMessage(const std::string& msg, std::promise<bool> &promise);

            void startWebsocketThread();

            void stopWebsocketThread();

            bool websocketThreadIsRunning();

            void disconnectConnectedClient();

            void getMessage(std::string &msg);

            bool hasMessages();
    };
}

#endif //DRONE_WEBSOCKET_H