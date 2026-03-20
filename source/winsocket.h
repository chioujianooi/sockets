#pragma once
#include "socket.h"
#include <winsock2.h>


class WinSocket : public Socket {
public:
    WinSocket(int port, std::string ip, int protocol);
    ~WinSocket();

    bool create() override;
    Socket* customAaccept() override;
    bool customBind() override;
    bool customListen(int clients) override;
    void customConnect(int port, std::string ip) override;

private:
    SOCKET socket_=0;
    SOCKET acceptSocket_=0;
};