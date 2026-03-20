#pragma once
#include <string>

class Socket {
public:
    
    Socket(int port, std::string ip, int protocol);
    virtual ~Socket();
    
    virtual bool create()=0;
    virtual bool customBind()=0;
    virtual bool customListen(int clients)=0;
    virtual Socket*customAaccept()=0;
    virtual void customConnect(int port, std::string ip)=0;

    protected:
    int port_;
    std::string ip_;
    int protocol_;
    
};