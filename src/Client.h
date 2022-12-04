#pragma once

#include "Net_Socket.h"



class Client : Net_Socket
{

public:

	Client(const char* _addressServer, USHORT _portServer, USHORT _portClient, class MyApp* _main);

	void SendMessageToServer(std::string _message);

protected:

	void Connect();

	void HandleConnection_Recv(int _socket, sockaddr_in _address);

	void HandleConnection_Send(int _socket, sockaddr_in _address);

	void ProcessPacket(std::string _packet, int _packet_length);

	void Login(FCommand_Packet _packet);

	USHORT ServerPort;
	const char* ServerAddress;
	sockaddr_in receiverAddr;
	class MyApp* main;
};

