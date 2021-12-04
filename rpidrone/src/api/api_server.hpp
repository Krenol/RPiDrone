#ifndef DRONE_API_APISERVER_H
#define DRONE_API_APISERVER_H

#include <string>

#define PYTHON_EXEC_PATH "/usr/bin/python3"

namespace drone {
    namespace api {
        class ApiServer {
            private:
                int pid_;

            public:
                void startApiServer(const std::string &pyfile);

                void stopApiServer();
        };
    }
}

#endif