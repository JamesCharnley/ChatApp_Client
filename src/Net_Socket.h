#pragma once

#include <stdio.h>
#include <winsock.h>
#include <thread>
#include <iostream>
#include <AppCore/AppCore.h>
#include <vector>

#define BUFFER_SIZE 100

struct FUser
{
public:
	std::string Username;
};

class Net_Socket
{

protected:

	Net_Socket();

	// the main socket that both the eserver and client will need.
	int Socket = 0;

	// address
	sockaddr_in Address;

	std::vector<std::string> out_queue = std::vector<std::string>();
	bool reciever_ready;

protected:

	// initialization of winodws socket functions
	void InitWSA(void);

	// Create Socket and address
	void CreateSocket(USHORT _port);

	//Bind Socket to address
	void BindSocket();

	bool ClientShuttingDown = false;

	USHORT LocalPort;
};

