// ServoClient.h
#ifndef SERVO_CLIENT_H
#define SERVO_CLIENT_H

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class ServoClient {
private:
    int sock;
    struct sockaddr_in server;

public:
    ServoClient(const std::string &ip, int port);
    ~ServoClient();
    bool connectToServer();
    bool sendAngle(int angle);
};

#endif
