#include "api_server.hpp"
#include <stdexcept>
#include <stdlib.h>
#include "../globals.hpp"

namespace drone
{
    namespace api
    {
        void ApiServer::startApiServer() 
        {
            std::string cmd = PYTHON_EXEC_PATH + " " + HOME_DIR + "/api/main.py &";
            pid_ = system(cmd.c_str());
            if(pid_ < 0) throw std::runtime_error("Error while starting api server");
        }
        
        void ApiServer::stopApiServer() 
        {
            auto cmd = "kill -9 " + std::to_string(pid_);
            system(cmd.c_str());
        }
    }
}