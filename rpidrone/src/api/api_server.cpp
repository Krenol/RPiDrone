#include "api_server.hpp"
#include "../logs/easylogging++.h"
#include <exception>
#include <stdlib.h>

namespace drone
{
    namespace api
    {
        void ApiServer::startApiServer(const std::string &pyfile) 
        {
            pid_ = system(pyfile.c_str());
            if(pid_ < 0) throw std::runtime_error("Error while starting api server");
        }
        
        void ApiServer::stopApiServer() 
        {
            auto cmd = "kill -9 " + std::to_string(pid_);
            system(cmd.c_str());
        }
    }
}