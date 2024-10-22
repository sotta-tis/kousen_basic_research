// ServoClient.cpp
#include "servoClient.h"
#include <iostream>

ServoClient::ServoClient(const std::string &ip, int port) {
    // ソケットの作成
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Could not create socket" << std::endl;
    }

    // サーバの設定
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}

ServoClient::~ServoClient() {
    close(sock); // ソケットのクローズ
}

bool ServoClient::connectToServer() {
    // サーバに接続
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Connect failed." << std::endl;
        return false;
    }
    std::cout << "Connected\n";
    return true;
}

bool ServoClient::sendAngle(int angle) {
    // 角度を文字列に変換して送信
    std::string message = std::to_string(angle) + "\n";
    if (send(sock, message.c_str(), message.size(), 0) < 0) {
        std::cerr << "Send failed" << std::endl;
        return false;
    }
    return true;
}
