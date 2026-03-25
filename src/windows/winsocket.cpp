
#include "winsocket.h"
#include <ws2tcpip.h>
#include <iostream>
#include <unordered_set>

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

int sendDataGeneral(const SOCKET& s, const char* data, int size) {
    int bytesSent = send(s, data, size, 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error at send(): " << WSAGetLastError() << std::endl;
    }
    return bytesSent;
}

int receiveDataGeneral(const SOCKET& s, char* buffer, int size) {
    int bytesReceived = recv(s, buffer, size, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Error at receive(): " << WSAGetLastError() << std::endl;
    }
    return bytesReceived;
}

struct WinServerSocket::Impl {
    SOCKET socket_;
    int clientCount;
    std::unordered_set<SOCKET> clientSockets; // Set to store client sockets
};

WinServerSocket::WinServerSocket() : ServerSocket(), pImpl_(new Impl()) {
    // Initialize Winsock
   
}

WinServerSocket::~WinServerSocket() {

    closesocket(pImpl_->socket_);
    delete pImpl_;
    WSACleanup();
}

bool WinServerSocket::create() {
    return createSocket(pImpl_->socket_);
}

bool WinServerSocket::customBind(int port, const char* ip, int ipSize) {
    auto service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if (bind(pImpl_->socket_, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Error at bind(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
        return false;
    }
    return true; // Placeholder
}

bool WinServerSocket::customListen(int clients) {
     if (listen(pImpl_->socket_, clients) == SOCKET_ERROR) {
        std::cerr << "Error at listen(): " << WSAGetLastError() << std::endl;

        //TODO: should we close the socket here?
        return false;
    }
    return true; // Placeholder
}

unsigned long long WinServerSocket::customAccept() {
    
    auto acceptedSocket= accept(pImpl_->socket_, nullptr,nullptr);
    if(acceptedSocket == INVALID_SOCKET) {
        std::cerr << "Error at accept(): " << WSAGetLastError() << std::endl;
        //TODO: should we close the socket here?
        return 0;
    }
   
    pImpl_->clientSockets.insert(acceptedSocket); // Store the accepted socket in the set
    return static_cast<unsigned long long>(acceptedSocket); // Placeholder
}



int WinServerSocket::sendData(const char* data, int size, unsigned long long socket) {
    auto it = pImpl_->clientSockets.find(socket);
    if (it == pImpl_->clientSockets.end()) {
        std::cerr << "Client with socket " << socket << " not found." << std::endl;
        return -1; // Client not found
    }
    return sendDataGeneral(*it, data, size);
}


int WinServerSocket::receiveData(char* buffer, int size, unsigned long long socket) {
    auto it = pImpl_->clientSockets.find(socket);
    if (it == pImpl_->clientSockets.end()) {
        std::cerr << "Client with socket " << socket << " not found." << std::endl;
        return -1; // Client not found
    }
    return receiveDataGeneral(*it, buffer, size);
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
    }
    // Implement customConnect logic here
}

int WinClientSocket::sendData(const char* data, int size) {
    return sendDataGeneral(socket_, data, size);
}

int WinClientSocket::receiveData(char* buffer, int size) {
    return receiveDataGeneral(socket_, buffer, size);
}
