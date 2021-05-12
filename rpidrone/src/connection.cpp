#include "connection.hpp"
#include "logs/easylogging++.h"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log

namespace drone
{
    void Connection::loop()
    {
        std::string msg, buf;
        int max_buf_size = buf.max_size() * 3 / 4;
        while (thread_on_)
        {
            NETWORK_LOG(INFO) << "waiting for connection on " << server_->getServerAddress() << ":" << server_->getServerPort();
            server_->connect();
            buf = "";
            NETWORK_LOG(INFO) << "connected to " << server_->getConnectedClient();
            while (server_->hasConnection() && thread_on_)
            {
                if (buf.size() > max_buf_size)
                {
                    NETWORK_LOG(WARNING) << "socket read buffer about to overflow! dumping buffer";
                    buf.clear();
                }
                try
                {
                    server_->readBytes(msg);
                    buf += msg;
                    processServerRead(buf);
                }
                catch (const std::exception &exc)
                {
                    NETWORK_LOG(ERROR) << exc.what();
                    break;
                }
            }
            queue_->clear();
            if (thread_on_)
            {
                queue_->push("{\"disconnected\": true}");
            }
            else
            {
                server_->disconnect();
                return;
            }
        }
    }

    void Connection::processServerRead(std::string &buf)
    {
        std::string msg;
        std::size_t pos, pos1;
        while ((pos = buf.find(delimiter_)) != std::string::npos)
        {
            msg = buf.substr(0, pos);
            pos1 = msg.find_first_of("{");
            msg.erase(0, pos1);
            if(queue_->item_count() > max_q_size_) queue_->clear();
            queue_->push(msg);
            buf.erase(0, pos + delimiter_.length());
        }
    }

    Connection::~Connection()
    {
        stopThread();
    }

    void Connection::startThread()
    {
        if (thread_on_)
            return;
        thread_on_ = true;
        thread_ = std::thread(&Connection::loop, this);
    }

    void Connection::stopThread()
    {
        if (thread_on_)
        {
            server_->disconnect();
            thread_on_ = false;
            thread_.join();
        }
    }

    bool Connection::hasConnection() const
    {
        return server_->hasConnection();
    }

    Connection::Connection(std::unique_ptr<rpisocket::Server> server, std::string delimiter, design_patterns::Queue<std::string>* queue, int queue_size) : delimiter_{delimiter}, server_{std::move(server)}, thread_on_{false}, max_q_size_{queue_size}
    {
        queue_ = queue;
    }

    void Connection::writeMsg(const std::string &msg)
    {
        try
        {
            if (hasConnection())
            {
                server_->writeBytes(msg);
            }
            else
            {
                NETWORK_LOG(WARNING) << "Cannot write to socket, as no client is connected";
            }
        }
        catch (const std::exception &exc)
        {
            NETWORK_LOG(ERROR) << exc.what();
        }
    }
}