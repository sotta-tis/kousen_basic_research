#pragma once

#include <drogon/WebSocketController.h>
#include <set>
#include <mutex>

using namespace drogon;

class adminWebSocket : public drogon::WebSocketController<adminWebSocket>
{
public:
    void handleNewMessage(const WebSocketConnectionPtr& connPtr,
                          std::string&& message,
                          const WebSocketMessageType& type) override;

    void handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& connPtr) override;

    void handleConnectionClosed(const WebSocketConnectionPtr& connPtr) override;

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/admi", Get);
    WS_PATH_LIST_END

};
