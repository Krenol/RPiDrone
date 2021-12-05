#include "websocket.hpp"
#include "logs/easylogging++.h"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log

namespace drone
{
    void Websocket::onIncomingMessage(const std::string& msg) 
    {
        if (queue_.item_count() > q_size_)
        {
            queue_.pop();
        }
        queue_.push(msg);
    }
    
    void Websocket::onConnectionOpening(std::shared_ptr<WsServer::Connection> connection) 
    {
        if (!connected_)
        {
            connection_ = connection;
            connected_ = true;
            NETWORK_LOG(INFO) << "Connected to " << connection->remote_endpoint().address().to_string();
        }
        else
        {
            //allow only 1 client
            connection->close();
        }
    }

    void Websocket::onConnectionClosing(std::shared_ptr<WsServer::Connection> connection, int status) 
    {
        if (connection->remote_endpoint().address().to_string() == connection_->remote_endpoint().address().to_string())
        {
            NETWORK_LOG(INFO) << "Server: Closed connection " << connection->remote_endpoint().address().to_string() << " with status code " << status;
            connected_ = false;
        }
    }
    
    void Websocket::onEndpointErrors(std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) 
    {
        NETWORK_LOG(INFO) << "Server: Error in connection " << connection->remote_endpoint().address().to_string() << ". "
                              << "Error: " << ec << ", error message: " << ec.message();
    }
    
    void Websocket::setupWebsocketEndpointEvents(WsEndpoint &endpoint) 
    {
        endpoint.on_message = [this](std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::InMessage> in_message)
        {
            onIncomingMessage(in_message->string());
        };

        endpoint.on_handshake = [this](std::shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/)
        {
            return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
        };

        endpoint.on_open = [this](std::shared_ptr<WsServer::Connection> connection)
        {
            onConnectionOpening(connection);
        };

        endpoint.on_close = [this](std::shared_ptr<WsServer::Connection> connection, int status, const std::string & /*reason*/)
        {
            onConnectionClosing(connection, status);
        };

        endpoint.on_error = [this](std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec)
        {
            onEndpointErrors(connection, ec);
        };
    }
    
    void Websocket::onMessageSendError(const SimpleWeb::error_code &ec) 
    {
        // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
        NETWORK_LOG(INFO) << "Server: Error sending message. " <<
            "Error: " << ec << ", error message: " << ec.message();
        disconnectConnectedClient();
    }
    
    void Websocket::sendMessageToConnectedClient(const std::string &msg, std::promise<bool> &promise) 
    {
        connection_->send(msg, [this, &promise](const SimpleWeb::error_code &ec)
            {
                if (ec) {
                    onMessageSendError(ec);
                    promise.set_value(false);
                } else {
                    promise.set_value(true);
                }
            }
        );
    }

    Websocket::Websocket(int q_size) : q_size_{q_size}
    {
    }

    void Websocket::init(const std::string &path, int port)
    {
        server_.endpoint.clear();
        server_.config.port = port;
        WsEndpoint &endpoint = server_.endpoint[path];
        setupWebsocketEndpointEvents(endpoint);
    }

    bool Websocket::hasConnection() const
    {
        return connected_;
    }

    std::future<bool> Websocket::writeMessage(const std::string &msg)
    {
        std::promise<bool> promise;
        if (connected_) {
            sendMessageToConnectedClient(msg, promise);
        } else {
            promise.set_value(false);
        }
        return promise.get_future();
    }

    void Websocket::disconnectConnectedClient()
    {
        connected_ = false;
        connection_->close();
    }

    void Websocket::startWebsocketThread()
    {
        srv_thrd_ = std::thread([this]()
        {
            server_.start();
        });
    }
    
    void Websocket::stopWebsocketThread() 
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