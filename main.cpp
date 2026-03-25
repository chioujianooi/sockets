
#include <iostream>
#include "posixsocket.h"

int main() {
    PosixClientSocket client;
    if (!client.create()) {
        std::cerr << "Failed to create server socket." << std::endl;
        return 1;
    }


    client.customConnect(8080,"192.168.56.1", 9);
    std::string message = "Hello from clientvery long messagevery long message long long long long long long long long gjkdfhgjrtiurzhgjkdfhgjkfdhgjdfkghureizturiehgjkdfjdfkghm,vcnbm,bndfjkghdkfghiuertzreiouteriogjdfklgdflkgjdfklgjdkflnbvmf,cbncvm,gjkdflgerioutreitureiotureiojgdkflgjkdlfnbvmc,bnmvc,jdkflgjieroutreiotureiojkldfgjkdflgnm,cvnbmvc,bnkdljfgdkfleiroutreiotueriotuiofgkldjfgklnm,cbvnmc,bnkdlfgjkdflgjkdflgjfdklgjkfdgldfireotuireotureioturieotuireoturieotuireoturieoturieotureiotuireotureiotuierotuieortuireotuioeruitoreutioreutioreutiroeutreiotueriotureiotureiotuireo! end";
    message += message;
    std::cout<< "max message size: " << message.length() << std::endl;
    client.sendData(message.data(), message.length());
    client.sendData(message.data(), message.length());
    char buffer[1024];
    client.receiveData(buffer, sizeof(buffer));

    std::cout<< "Received data: " << buffer << std::endl;
    return 0;
}