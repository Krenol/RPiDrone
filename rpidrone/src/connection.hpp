#ifndef DRONE_CONNECTION_H
#define DRONE_CONNECTION_H

#include <queue>
#include "rpisocket/rpisocket.hpp"
#include <memory>
#include <string>

namespace drone
{
    class Connection {
    private:
        const std::string delimiter_;
        std::thread thread_;
        std::atomic_bool thread_on_;
        std::unique_ptr<rpisocket::Server> server_;
        design_patterns::Queue<std::string>* queue_;
        int max_q_size_;

        /**
         * Loop executed in thread
         */
        void loop();

        /**
         * Method that is used to parse the read string
         * @param buf the current buffer
         */
        void processServerRead(std::string& buf);
        
    public:

        Connection(std::unique_ptr<rpisocket::Server> server, std::string delimiter, design_patterns::Queue<std::string>* queue, int queue_size);
        ~Connection();

        /**
         * start the connection handler thread
         */
        void startThread();

        /**
         * stop the connection handler thread
         */
        void stopThread();

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
    };
}

#endif