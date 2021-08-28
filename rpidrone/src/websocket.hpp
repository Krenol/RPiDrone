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

        public:
            Websocket(int q_size);

            /**
             * @brief init websocket
             * 
             * @param path the path on which is to be listened
             * @param port the port of the websocket
             */
            void init(const std::string &path, int port);

            /**
             * Method to check if server has connection or not
             * @return true if server has connected clients else false
             */
            bool hasConnection() const;

            /**
             * Write a message to the connected clients
             * @param msg The message to be written
             */
            void writeMsg(const std::string& msg);

            /**
             * @brief start the websocket server
             * 
             */
            void start();

            /**
             * @brief stop the websocket server
             * 
             */
            void stop();

            /**
             * @brief Get the next received msg from the queue
             * 
             * @param msg The string where the msg is to be stored
             */
            void getMsg(std::string &msg);

            /**
             * @brief check if queue has messages stored
             * 
             * @return true if messages are available
             * @return false no messages available
             */
            bool hasMessages();
    };
}

#endif //DRONE_WEBSOCKET_H