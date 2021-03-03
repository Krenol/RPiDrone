#include "design_patterns/design_patterns.hpp"
#include "rpisocket/rpisocket.hpp"
#include <memory>
#include <string>
#include <atomic>

#ifndef DRONE_CONNECTION_H
#define DRONE_CONNECTION_H

namespace drone
{
    class Connection : public design_patterns::Publisher<std::string> {
    private:
        const std::string delimiter_;
        std::thread thread_;
        std::atomic_bool thread_on_;
        std::unique_ptr<rpisocket::WiFiServer> server_;

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

        Connection(int port, int msg_size, std::string delimiter);
        ~Connection();

        void startThread();

        void stopThread();
    };
}

#endif