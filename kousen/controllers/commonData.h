#pragma once

#include <drogon/WebSocketConnection.h>
#include <set>
#include <mutex>

namespace commonData{
    extern std::set<drogon::WebSocketConnectionPtr> clients;  // clientsセットを外部からアクセス可能に
    extern std::mutex clientsMutex;

    void sentMessageToWebsockets(const std::string &message);
}