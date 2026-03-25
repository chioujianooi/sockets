#include "posixsocket.h"

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

struct PosixServerSocket::Impl
{
    int socket_ = -1;
    std::unordered_set<int> clientSockets_;
};

PosixServerSocket::PosixServerSocket(): impl_(new Impl())
{
}

PosixServerSocket::~PosixServerSocket()
{
    if(impl_->socket_ != -1) {
        close(impl_->socket_);
    }
    delete impl_;
}

bool PosixServerSocket::create()
{
    impl_->socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(impl_->socket_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    return true;
}

bool PosixServerSocket::customBind(int port, const char *ip, int ipSize)
{
    sockaddr_in service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if(bind(impl_->socket_, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return false;
    }
    return true;
}

bool PosixServerSocket::customListen(int clients)
{
    if(listen(impl_->socket_, clients) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return false;
    }
    return true;
}

unsigned long long PosixServerSocket::customAccept()
{
    int clientSocket = accept(impl_->socket_, nullptr, nullptr);
    if(clientSocket == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        return 0;
    }
    impl_->clientSockets_.insert(clientSocket);
    return static_cast<unsigned long long>(clientSocket);
}

int PosixServerSocket::sendData(const char *data, int size, unsigned long long socket)
{
    int clientSocket = static_cast<int>(socket);
    if(impl_->clientSockets_.find(clientSocket) == impl_->clientSockets_.end()) {
        std::cerr << "Invalid client socket" << std::endl;
        return -1;
    }
    auto sentBytes= send(clientSocket, data, size, 0);
    if(sentBytes == -1) {
        std::cerr << "Failed to send data" << std::endl;
    }
    return sentBytes;
}

int PosixServerSocket::receiveData(char *buffer, int size, unsigned long long socket)
{
    int clientSocket = static_cast<int>(socket);
    if(impl_->clientSockets_.find(clientSocket) == impl_->clientSockets_.end()) {
        std::cerr << "Invalid client socket" << std::endl;
        return -1;
    }
    auto receivedBytes = recv(clientSocket, buffer, size, 0);
    if(receivedBytes == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    }
    return receivedBytes;
}

PosixClientSocket::PosixClientSocket(): socket_(-1)
{
}

bool PosixClientSocket::create()
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    return true;
}

void PosixClientSocket::customConnect(int port, const char *ip, int ipSize)
{
    sockaddr_in service = generateSockaddr_in(port, std::string_view(ip, ipSize));
    if(connect(socket_, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
    }
}

int PosixClientSocket::sendData(const char *data, int size)
{
    auto sentBytes = send(socket_, data, size, 0);
    if(sentBytes == -1) {
        std::cerr << "Failed to send data" << std::endl;
    }
    return sentBytes;
}

int PosixClientSocket::receiveData(char *buffer, int size)
{
    auto receivedBytes = recv(socket_, buffer, size, 0);
    if(receivedBytes == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    }
    return receivedBytes;
}
