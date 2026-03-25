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
    ServerSocket();
    ~ServerSocket() override;
    bool create() override;
    bool customBind(int port, const char* ip, int ipSize);
    bool customListen(int clients);
    unsigned long long customAccept();
    int sendData(const char* data, int size,unsigned long long socket);
    int receiveData(char* buffer, int size, unsigned long long socket);
private:
    struct Impl;
    Impl* pImpl_;
};

class MYSOCKET ClientSocket : public Socket {
public:    
    ClientSocket();
    ~ClientSocket() override;
    bool create() override;
    void customConnect(int port, const char* ip, int ipSize);
    int sendData(const char* data, int size);
    int receiveData(char* buffer, int size);
private:
    struct Impl;
    Impl* pImpl_;
};

