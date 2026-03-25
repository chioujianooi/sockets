#include "socket.h"
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unordered_set>

sockaddr_in generateSockaddr_in(int port, std::string_view ip)
{
    sockaddr_in service;
    service.sin_family = AF_INET;
    inet_pton(AF_INET, ip.data(), &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    return service;
}

struct ServerSocket::Impl
{
    int socket_ = -1;
    std::unordered_set<int> clientSockets_;
};

ServerSocket::ServerSocket(): pImpl_(new Impl())
{
}

ServerSocket::~ServerSocket()
{
    if(pImpl_->socket_ != -1) {
        close(pImpl_->socket_);
    }
    delete pImpl_;
}

bool ServerSocket::create()
{
    pImpl_->socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(pImpl_->socket_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    return true;
}

bool ServerSocket::customBind(int port, const char *ip, int ipSize)
{
    sockaddr_in service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if(bind(pImpl_->socket_, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return false;
    }
    return true;
}

bool ServerSocket::customListen(int clients)
{
    if(listen(pImpl_->socket_, clients) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return false;
    }
    return true;
}

unsigned long long ServerSocket::customAccept()
{
    int clientSocket = accept(pImpl_->socket_, nullptr, nullptr);
    if(clientSocket == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        return 0;
    }
    pImpl_->clientSockets_.insert(clientSocket);
    return static_cast<unsigned long long>(clientSocket);
}

int ServerSocket::sendData(const char *data, int size, unsigned long long socket)
{
    int clientSocket = static_cast<int>(socket);
    if(pImpl_->clientSockets_.find(clientSocket) == pImpl_->clientSockets_.end()) {
        std::cerr << "Invalid client socket" << std::endl;
        return -1;
    }
    auto sentBytes= send(clientSocket, data, size, 0);
    if(sentBytes == -1) {
        std::cerr << "Failed to send data" << std::endl;
    }
    return sentBytes;
}

int ServerSocket::receiveData(char *buffer, int size, unsigned long long socket)
{
    int clientSocket = static_cast<int>(socket);
    if(pImpl_->clientSockets_.find(clientSocket) == pImpl_->clientSockets_.end()) {
        std::cerr << "Invalid client socket" << std::endl;
        return -1;
    }
    auto receivedBytes = recv(clientSocket, buffer, size, 0);
    if(receivedBytes == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    }
    return receivedBytes;
}

struct ClientSocket::Impl
{
    int socket_ = -1;
};

ClientSocket::ClientSocket(): pImpl_(new Impl())
{
}

ClientSocket::~ClientSocket()
{
    if(pImpl_->socket_ != -1) {
        close(pImpl_->socket_);
    }
    delete pImpl_;
}

bool ClientSocket::create()
{
    pImpl_->socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(pImpl_->socket_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    return true;
}

void ClientSocket::customConnect(int port, const char *ip, int ipSize)
{
    sockaddr_in service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if(connect(pImpl_->socket_, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
    }
}

int ClientSocket::sendData(const char *data, int size)
{
    auto sentBytes = send(pImpl_->socket_, data, size, 0);
    if(sentBytes == -1) {
        std::cerr << "Failed to send data" << std::endl;
    }
    return sentBytes;
}

int ClientSocket::receiveData(char *buffer, int size)
{
    auto receivedBytes = recv(pImpl_->socket_, buffer, size, 0);
    if(receivedBytes == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    }
    return receivedBytes;
}
