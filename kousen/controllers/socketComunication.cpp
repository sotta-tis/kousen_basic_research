#include "socketComunication.h"

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"

using json = nlohmann::json;

void sockC::send_command(const std::string& command, const std::string& host , int port){
    int sock =0;
    struct sockaddr_in servAddr;
    char buffer[1024] = {0};

    // ソケットの作成
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cerr << "Cant Create Socket" << std::endl;
        return;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    if(inet_pton(AF_INET,host.c_str(), &servAddr.sin_addr) <= 0){
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return;
    }

    if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0){
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    send(sock, command.c_str(), command.length(), 0);
    std::cout << "Command sent: " << command << std::endl;

    int vafread = read(sock,buffer,1024);
    std::cout << "Response: " << std::string(buffer, vafread) << std::endl;

    close(sock);

}
void sockC::moveArmParam(int x, int y, int z,std::string host,int port) {
    // 接続確認
    json command;
    command["command"] = "Ping";
    std::string command_str = command.dump();
    sockC::send_command(command_str,host,port);

    // よーわからん
    command["command"] = "SetCordinateSpeed";
    command["velocity"] = 20;
    command["jerk"] = 3;
    command_str = command.dump();
    sockC::send_command(command_str,host,port);

    // 高さ？
    json com1;
    com1["command"] = "SetJumpPram";
    com1["height"] = 50;
    com1["zlimit"] = 185;
    command_str = com1.dump();
    sockC::send_command(command_str,host,port);

    json com2;
    com2["command"] = "JumpTo";
    com2["x"] = x;
    com2["y"] = y;
    com2["z"] = z;
    com2["r"] = 0;
    command_str = com2.dump();
    send_command(command_str,host,port);


}

//
// Created by 年澄荘多 on 24/07/03.
//
