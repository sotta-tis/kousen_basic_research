#include "commonData.h"
#include "socketServer.h"

namespace commonData{
    std::set<drogon::WebSocketConnectionPtr> clients;
    std::mutex clientsMutex;
    SocketServer servoS("aaa",8000);

    void sentMessageToWebsockets(const std::string &message)
    {
        std::lock_guard<std::mutex> guard(commonData::clientsMutex);
        for (const auto &client : commonData::clients)
        {
            if (client->connected())
            {
                client->send(message);
            }
        }
    }
}
