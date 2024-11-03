#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"


namespace sockC{
    void send_command(const std::string& command, const std::string& host , int port);

    void setting(std::string host,int port);
    void moveArmParam(int x, int y, int z,int r,std::string host,int port) ;
    void moveArmParamGo(int x, int y, int z,int r,std::string host,int port) ;
    void quit(std::string host,int port);
}