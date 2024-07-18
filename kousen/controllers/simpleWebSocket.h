#pragma once

#include <drogon/WebSocketController.h>

using namespace drogon;

class SimpleWebSocket : public WebSocketController<SimpleWebSocket>
{
    public:
    void handleNewMessage(const WebSocketConnectionPtr& connPtr,
                          std::string&& message,
                          const WebSocketMessageType& type) override;

    void handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& connPtr) override;

    void handleConnectionClosed(const WebSocketConnectionPtr& connPtr) override;

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/echo", Get);
    WS_PATH_LIST_END
};
