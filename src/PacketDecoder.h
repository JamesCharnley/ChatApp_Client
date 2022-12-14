#pragma once

#include <iostream>
#include <string>

enum class ECommand
{
	InValid,
	Login,
	Signup,
	Post,
	Get,
	Authorized,
	GetRoom
};

enum class ESub_Command
{
	InValid,
	Room,
	RoomList,
	Contacts
};

struct FCommand_Packet
{
public:

	ECommand Command;
	std::string Content;
};

struct FLogin_Packet
{
public:

	ECommand Command;
	std::string Username;
	std::string Password;
};

struct FString_Packet
{
public:

	ECommand Command;
	std::string _string;
};

struct FGet_Packet
{
public:

	ECommand Command;
	ESub_Command Sub_Command;
	std::string Content;

};

class PacketDecoder
{
public:

	static FCommand_Packet Char_To_Command_Packet(std::string _char, int _length);
	static std::string Command_Packet_To_String(FCommand_Packet _packet);
	static FLogin_Packet Command_Packet_To_Login_Packet(FCommand_Packet _com_packet);
	static std::string Login_Packet_To_String(FLogin_Packet _login_packet);
	static FString_Packet Command_Packet_To_String_Packet(FCommand_Packet _com_packet);
	static std::string String_Packet_To_String(FString_Packet _string_packet);
	static FGet_Packet Command_Packet_To_Get_Packet(FCommand_Packet _com_packet);
	static std::string Get_Packet_To_String(FGet_Packet _get_packet);
};
