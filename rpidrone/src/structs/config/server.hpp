#ifndef DRONE_STRUCT_CONFIG_SERVER_H
#define DRONE_STRUCT_CONFIG_SERVER_H
#include <string>

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct Server
            {
                int port = 8889, queue_size = 10;
                std::string context = "/", cert_dir = "";
                bool ssl = false;

                Server()
                {
                }

                Server(int port, int queue_size, const std::string &context, bool ssl, const std::string &cert_dir)
                {
                    this->port = port;
                    this->queue_size = queue_size;
                    this->context = context;
                    this->ssl = ssl;
                    this->cert_dir = cert_dir;
                }

                Server(const Server &s)
                {
                    this->port = s.port;
                    this->queue_size = s.queue_size;
                    this->context = s.context;
                    this->ssl = s.ssl;
                    this->cert_dir = s.cert_dir;
                }
            };
        }
    }
}
#endif