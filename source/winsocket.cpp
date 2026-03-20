#include "winsocket.h"
#include <ws2tcpip.h>
#include <iostream>

WinSocket::WinSocket() : Socket() {
    // Initialize Winsock
   
}

WinSocket::~WinSocket() {

    closesocket(socket_);
    WSACleanup();
}

bool WinSocket::create() {
     WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return false;
    }

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket and WSACleanup here?
        return false;
    }
    return true; // Placeholder
}

bool WinSocket::customBind(int port, std::string ip) {
    auto service = generateSockaddr_in(port, ip);
    if (bind(socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at bind(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
        return false;
    }
    return true; // Placeholder
}

bool WinSocket::customListen(int clients) {
     if (listen(socket_, clients) == SOCKET_ERROR) {
        std::cerr << "Error at listen(): " << WSAGetLastError() << std::endl;

        //TODO: should we close the socket here?
        return false;
    }
    return true; // Placeholder
}

Socket* WinSocket::customAccept() {
    acceptSocket_ = accept(socket_, nullptr, nullptr);
    if(acceptSocket_ == INVALID_SOCKET) {
        std::cerr << "Error at accept(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
        return nullptr;
    }
    return nullptr; // Placeholder
}

void WinSocket::customConnect(int port, std::string ip) {
    auto service = generateSockaddr_in(port, ip);
    if(connect(socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at connect(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
    }
    // Implement customConnect logic here
}

sockaddr_in WinSocket::generateSockaddr_in(int port, std::string ip)
{
    
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, ip.c_str(), &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    return service;
}
