#pragma once
#include <string>

class Socket {
public:
    
    Socket();
    virtual ~Socket();
    
    virtual bool create()=0;
    virtual bool customBind(int port, std::string ip)=0;
    virtual bool customListen(int clients)=0;
    virtual Socket*customAccept()=0;
    virtual void customConnect(int port, std::string ip)=0;
    
};