#ifndef DRONE_API_APISERVER_H
#define DRONE_API_APISERVER_H

#include <string>

namespace drone {
    namespace api {
        class ApiServer {
            private:
                int pid_;
                const std::string PYTHON_EXEC_PATH = "/usr/bin/python3";

            public:
                void startApiServer();

                void stopApiServer();
        };
    }
}

#endif