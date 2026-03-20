#pragma once
#include "socket.h"
#include <winsock2.h>


class WinSocket : public Socket {
public:
    WinSocket();
    ~WinSocket();

    bool create() override;
    Socket* customAccept() override;
    bool customBind(int port, std::string ip) override;
    bool customListen(int clients) override;
    void customConnect(int port, std::string ip) override;

private:
    sockaddr_in generateSockaddr_in(int port, std::string ip);
    SOCKET socket_=0;
    SOCKET acceptSocket_=0;
};