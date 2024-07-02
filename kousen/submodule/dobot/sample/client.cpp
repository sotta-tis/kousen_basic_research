#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"  // json.hppをプロジェクトディレクトリに配置

using json = nlohmann::json;

#define PORT 8893
#define BUFFER_SIZE 1024

void send_command(const std::string& command, const std::string& host = "172.0.0.1", int port = PORT) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    send(sock, command.c_str(), command.length(), 0);
    std::cout << "Command sent: " << command << std::endl;

    int valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "Response: " << std::string(buffer, valread) << std::endl;

    close(sock);
}

int main() {
    json command;
    command["command"] = "Ping";

    std::string command_str = command.dump();

    send_command(command_str);

    command["command"] = "SetCordinateSpeed";
    command["velocity"] = 20;
    command["jerk"] = 3;
    command_str = command.dump();

    send_command(command_str);

    command["command"] = "SetJumpPram";
    command["height"] = 50;
    command["zlimit"] = 185;
    command_str = command.dump();

    send_command(command_str);

    // 他のコマンドをここに追加...

    return 0;
}
