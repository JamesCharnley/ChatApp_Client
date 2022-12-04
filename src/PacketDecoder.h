#pragma once

#include <iostream>
#include <string>
#include <AppCore/AppCore.h>

enum class ECommand
{
	InValid,
	Login,
	Signup,
	Post,
	Get,
	Authorized
};

struct FCommand_Packet
{
public:

	ECommand Command;
	std::string Content;
};

struct FLogin_Packet
{
	ECommand Command;
	std::string Username;
	std::string Password;
};

struct FString_Packet
{
	ECommand Command;
	std::string _string;
};

class PacketDecoder
{
public:

	static FCommand_Packet Char_To_Command_Packet(std::string _char, int _length);
	static FLogin_Packet Command_Packet_To_Login_Packet(FCommand_Packet _com_packet);
	static FString_Packet Command_Packet_To_String_Packet(FCommand_Packet _com_packet);
};

