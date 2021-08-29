#include "websocket.hpp"
#include "logs/easylogging++.h"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log

namespace drone
{
    Websocket::Websocket(int q_size) : q_size_{q_size}
    {
    }

    void Websocket::init(const std::string &path, int port)
    {
        server_.endpoint.clear();
        server_.config.port = port;
        //server_.config.address = "0.0.0.0";
        WsEndpoint &endpoint = server_.endpoint[path];
        endpoint.on_message = [this](std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::InMessage> in_message)
        {
            auto out_message = in_message->string();
            if (queue_.item_count() > q_size_)
            {
                queue_.clear();
            }
            queue_.push(out_message);
        };

        endpoint.on_handshake = [](std::shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/)
        {
            return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
        };

        endpoint.on_open = [this](std::shared_ptr<WsServer::Connection> connection)
        {
            if (!connected_)
            {
                connection_ = connection;
                connected_ = true;
                NETWORK_LOG(INFO) << "Connected to " << connection->remote_endpoint().address().to_string();
            }
            else
            {
                connection->close();
            }
        };

        endpoint.on_close = [this](std::shared_ptr<WsServer::Connection> connection, int status, const std::string & /*reason*/)
        {
            if (connection->remote_endpoint().address().to_string() == connection_->remote_endpoint().address().to_string())
            {
                NETWORK_LOG(INFO) << "Server: Closed connection " << connection->remote_endpoint().address().to_string() << " with status code " << status;
                connection_.reset();
                connected_ = false;
            }
        };

        endpoint.on_error = [this](std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec)
        {
            NETWORK_LOG(INFO) << "Server: Error in connection " << connection->remote_endpoint().address().to_string() << ". "
                              << "Error: " << ec << ", error message: " << ec.message();
        };
    }

    bool Websocket::hasConnection() const
    {
        return connected_;
    }

    void Websocket::writeMessage(const std::string &msg)
    {
        if (connected_)
        {
            connection_->send(msg, [this](const SimpleWeb::error_code &ec)
                {
                    if (ec)
                    {
                        NETWORK_LOG(INFO) << "Server: Error sending message. " <<
                            // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                            "Error: " << ec << ", error message: " << ec.message();
                        disconnect();
                    }
                });
        }
    }

    void Websocket::disconnect()
    {
        connected_ = false;
        connection_->close();
        connection_.reset();
    }

    void Websocket::start()
    {
        srv_thrd_ = std::thread([this]()
        {
            server_.start();
        });
    }
    
    void Websocket::stop() 
    {
        server_.stop();
        srv_thrd_.join();
    }
    
    void Websocket::getMessage(std::string &msg) 
    {
        queue_.pop(msg);
    }
    
    bool Websocket::hasMessages() 
    {
        return queue_.item_count() > 0;
    }
}