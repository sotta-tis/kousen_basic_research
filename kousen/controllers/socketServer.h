#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <string>

class SocketServer {
public:
    SocketServer(const std::string& host, int port);
    void start();  // サーバーを開始
    void controlServoCommand(const std::string& command);  // サーボを制御する

private:
    std::string _host;
    int _port;
    void processClientRequest(int clientSock);  // クライアントリクエスト処理
};

#endif // SOCKETSERVER_H
