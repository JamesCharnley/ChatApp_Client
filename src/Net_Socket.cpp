#include <thread>
#include "Net_Socket.h"

Net_Socket::Net_Socket()
{
	
    Address = sockaddr_in();
   
}



void Net_Socket::InitWSA(void)
{
    WORD wVersionRequested;
    WSAData wsaData;

    wVersionRequested = MAKEWORD(1, 1);

    int nRc = WSAStartup(wVersionRequested, &wsaData);

    if (nRc != 0)
    {
        std::cout << stderr << "ERROR when calling WSAStartup\n";
        exit(-1);
    }

    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
    {
        std::cout << stderr << "Version not available\n";
        exit(-1);
    }
}

void Net_Socket::CreateSocket(USHORT _port)
{
    // create a socket
    Socket = socket(AF_INET, SOCK_STREAM, 0);

    if (Socket == -1)
    {
        std::cout << "Error in socket(). Error code: " << WSAGetLastError() << std::endl;
        exit(-1);
    }

    // set server Socket Address
    Address.sin_family = AF_INET;
    Address.sin_port = htons(_port);
    Address.sin_addr.s_addr = INADDR_ANY;
}

void Net_Socket::BindSocket()
{
    // Bind the address to the socket
    int status = bind(Socket, (struct sockaddr*)&Address, sizeof(struct sockaddr));

    if (status == -1)
    {
        std::cout << "ERROR in bind(): Error code: " << WSAGetLastError() << std::endl;
        exit(-1);
    }

}

