#include "sampleWebSocket.h"
#include <drogon/drogon.h>

void SampleWebSocket::handleNewMessage(const WebSocketConnectionPtr& connPtr,
                                       std::string&& message,
                                       const WebSocketMessageType& type)
{
    if (type == WebSocketMessageType::Text)
    {
        connPtr->send(message); // Echo the received message back to the client
    }
}

void SampleWebSocket::handleNewConnection(const HttpRequestPtr& req,
                                          const WebSocketConnectionPtr& connPtr)
{
    LOG_INFO << "New WebSocket connection";
}

void SampleWebSocket::handleConnectionClosed(const WebSocketConnectionPtr& connPtr)
{
    LOG_INFO << "WebSocket connection closed";
}
