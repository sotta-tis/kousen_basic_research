#include "socketServer.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

SocketServer::SocketServer(const std::string& host, int port) : _host(host), _port(port) {}

void SocketServer::start() {
    int serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buffer[1024] = {0};

    // サーバーソケットの作成
    if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Failed to create server socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    // ソケットのバインド
    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind server socket" << std::endl;
        return;
    }

    // 接続待機
    if (listen(serverSock, 3) < 0) {
        std::cerr << "Failed to listen on server socket" << std::endl;
        return;
    }

    std::cout << "Server started. Waiting for connections..." << std::endl;

    // クライアントからの接続待機
    while ((clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrSize)) >= 0) {
        std::cout << "Client connected" << std::endl;

        int valread = read(clientSock, buffer, 1024);
        std::string command(buffer, valread);

        // クライアントにコマンドを送信
        controlServoCommand(command);

        close(clientSock);
        std::cout << "Client disconnected" << std::endl;
    }
}

void SocketServer::controlServoCommand(const std::string& command) {
    if (command == "open") {
        std::cout << "Sending open command" << std::endl;
        // ESP32に"open"コマンドを送信
    } else if (command == "close") {
        std::cout << "Sending close command" << std::endl;
        // ESP32に"close"コマンドを送信
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}