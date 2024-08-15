#include "adminWebSocket.h"
#include <drogon/drogon.h>

void adminWebSocket::handleNewMessage(const WebSocketConnectionPtr& connPtr,
                                       std::string&& message,
                                       const WebSocketMessageType& type)
{
    if (type == WebSocketMessageType::Text)
    {
        connPtr->send(message); // Echo the received message back to the client
    }
}

void adminWebSocket::handleNewConnection(const HttpRequestPtr& req,
                                          const WebSocketConnectionPtr& connPtr)
{
    {
        std::lock_guard<std::mutex> guard(clientsMutex_);
        clients_.insert(connPtr);
    }
    LOG_INFO << "New WebSocket connection";
}

void adminWebSocket::handleConnectionClosed(const WebSocketConnectionPtr& connPtr)
{
    {
        std::lock_guard<std::mutex> guard(clientsMutex_);
        clients_.erase(connPtr);
    }
    LOG_INFO << "WebSocket connection closed";
}

void adminWebSocket::broadcastMessage(const std::string &message)
{
    std::lock_guard<std::mutex> guard(clientsMutex_);
    for (const auto &client : clients_)
    {
        if (client->connected())
        {
            client->send(message);
        }
    }
}