
#include "winsocket.h"
#include <ws2tcpip.h>
#include <iostream>

sockaddr_in generateSockaddr_in(int port, std::string_view ip)
{
    
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, ip.data(), &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    return service;
}

bool createSocket(SOCKET& s) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return false;
    }

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

int sendDataGeneral(SOCKET& s, const char* data, int size) {
    int bytesSent = send(s, data, size, 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error at send(): " << WSAGetLastError() << std::endl;
    }
    return bytesSent;
}

int receiveDataGeneral(SOCKET& s, char* buffer, int size) {
    int bytesReceived = recv(s, buffer, size, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Error at receive(): " << WSAGetLastError() << std::endl;
    }
    return bytesReceived;
}

WinServerSocket::WinServerSocket() : ServerSocket() {
    // Initialize Winsock
   
}

WinServerSocket::~WinServerSocket() {

    closesocket(socket_);
    WSACleanup();
}

bool WinServerSocket::create() {
    return createSocket(socket_);
}

bool WinServerSocket::customBind(int port, const char* ip, int ipSize) {
    auto service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if (bind(socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at bind(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
        return false;
    }
    return true; // Placeholder
}

bool WinServerSocket::customListen(int clients) {
     if (listen(socket_, clients) == SOCKET_ERROR) {
        std::cerr << "Error at listen(): " << WSAGetLastError() << std::endl;

        //TODO: should we close the socket here?
        return false;
    }
    return true; // Placeholder
}

Socket* WinServerSocket::customAccept() {
    acceptSocket_ = accept(socket_, nullptr, nullptr);
    if(acceptSocket_ == INVALID_SOCKET) {
        std::cerr << "Error at accept(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
        return nullptr;
    }
    return nullptr; // Placeholder
}



int WinServerSocket::sendData(const char* data, int size) {
    return sendDataGeneral(socket_, data, size);
}


int WinServerSocket::receiveData(char* buffer, int size) {
    return receiveDataGeneral(acceptSocket_, buffer, size);
}

bool WinClientSocket::create()
{
    return createSocket(socket_);
}

WinClientSocket::WinClientSocket() : ClientSocket() {
    // Initialize Winsock
}

WinClientSocket::~WinClientSocket() {
    closesocket(socket_);
    WSACleanup();
}

void WinClientSocket::customConnect(int port, const char* ip, int ipSize)
{
    auto service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if(connect(socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at connect(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
    }
    // Implement customConnect logic here
}

int WinClientSocket::sendData(const char* data, int size) {
    return sendDataGeneral(socket_, data, size);
}

int WinClientSocket::receiveData(char* buffer, int size) {
    return receiveDataGeneral(socket_, buffer, size);
}
