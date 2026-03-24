#pragma once

#ifdef _WIN32
  #ifdef MYSOCKET_EXPORTS
    #define MYSOCKET __declspec(dllexport)
  #else
    #define MYSOCKET __declspec(dllimport)
  #endif
#else
  #define MYSOCKET
#endif

class MYSOCKET Socket {
public:
    
    
    virtual ~Socket(){};
    virtual bool create()=0;
    

protected:
     Socket() = default;
};

class MYSOCKET ServerSocket : public Socket {
public:
    ServerSocket() = default;
    ~ServerSocket() override = default;
    virtual bool customBind(int port, const char* ip, int ipSize)=0;
    virtual bool customListen(int clients)=0;
    virtual unsigned long long customAccept()=0;
    virtual int sendData(const char* data, int size,unsigned long long socket)=0;
    virtual int receiveData(char* buffer, int size, unsigned long long socket)=0;
};

class MYSOCKET ClientSocket : public Socket {
public:    
    ClientSocket() = default;
    ~ClientSocket() override = default;
    virtual void customConnect(int port, const char* ip, int ipSize)=0;
    virtual int sendData(const char* data, int size)=0;
    virtual int receiveData(char* buffer, int size)=0;
};