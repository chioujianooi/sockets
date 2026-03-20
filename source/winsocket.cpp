#include "winsocket.h"
#include <ws2tcpip.h>
#include <iostream>

WinSocket::WinSocket(int port, std::string ip, int protocol) : Socket(port, ip, protocol) {
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
        WSACleanup();
        return false;
    }
    return true; // Placeholder
}

bool WinSocket::customBind() {
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, ip_.c_str(), &service.sin_addr.s_addr);
    service.sin_port = htons(port_);
    if (bind(socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at bind(): " << WSAGetLastError() << std::endl;
        return false;
    }
    return true; // Placeholder
}

bool WinSocket::customListen(int clients) {
     if (listen(socket_, clients) == SOCKET_ERROR) {
        std::cerr << "Error at listen(): " << WSAGetLastError() << std::endl;
        return false;
    }
    return true; // Placeholder
}

Socket* WinSocket::customAaccept() {
    acceptSocket_ = accept(socket_, nullptr, nullptr);
    if(acceptSocket_ == INVALID_SOCKET) {
        std::cerr << "Error at accept(): " << WSAGetLastError() << std::endl;
        return nullptr;
    }
    return nullptr; // Placeholder
}

void WinSocket::customConnect(int port, std::string ip) {
     sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, ip.c_str(), &service.sin_addr.s_addr);
    service.sin_port = htons(port);


    if(connect(socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at connect(): " << WSAGetLastError() << std::endl;
    }
    // Implement customConnect logic here
}