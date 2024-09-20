#pragma once

#include <drogon/WebSocketConnection.h>
#include <set>
#include <mutex>
#include "socketServer.h"

namespace commonData{
    extern std::set<drogon::WebSocketConnectionPtr> clients;  // clientsセットを外部からアクセス可能に
    extern std::mutex clientsMutex;

    extern SocketServer servoS;

    void sentMessageToWebsockets(const std::string &message);
}