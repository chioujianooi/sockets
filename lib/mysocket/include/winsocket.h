#pragma once
#include "socket.h"
#include <winsock2.h>


class MYSOCKET WinServerSocket : public ServerSocket {
public:
    WinServerSocket();
    ~WinServerSocket();

    bool create() override;
    char* customAccept() override;
    bool customBind(int port, const char* ip, int ipSize) override;
    bool customListen(int clients) override;
    
    int sendData(const char* data, int size, const char* ip) override;
    int receiveData(char* buffer, int size, const char* ip) override;

private:
    struct Impl;
    Impl* pImpl_;
};


class MYSOCKET WinClientSocket : public ClientSocket {
public:
    WinClientSocket();
    ~WinClientSocket();

    bool create() override;
    void customConnect(int port, const char* ip, int ipSize) override;
    int sendData(const char* data, int size) override;
    int receiveData(char* buffer, int size) override;   
private:
    SOCKET socket_=0;
};