#pragma once
#include "socket.h"

class MYSOCKET PosixServerSocket : public ServerSocket {
public:
    PosixServerSocket();
    ~PosixServerSocket();
    bool create() override;
    bool customBind(int port, const char* ip, int ipSize) override;
    bool customListen(int clients) override;
    unsigned long long customAccept() override;
    int sendData(const char* data, int size, unsigned long long socket) override;
    int receiveData(char* buffer, int size, unsigned long long socket) override;

private:
    struct Impl;
    Impl* impl_;
};

class MYSOCKET PosixClientSocket : public ClientSocket{
public:
    PosixClientSocket();
    ~PosixClientSocket() =default;
    bool create() override;
    void customConnect(int port, const char* ip, int ipSize) override;
    int sendData(const char* data, int size) override;
    int receiveData(char* buffer, int size) override;
private:
    int socket_;
};