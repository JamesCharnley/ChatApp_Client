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
	Authorized
};

enum class ESub_Command
{
	InValid,
	Room,
	RoomList,
	Contacts,
	Message
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

struct FGet_Post_Packet
{
public:

	ECommand Command;
	ESub_Command Sub_Command;
	std::string Content;

};

struct FPost_Message_Packet
{
public:

	ECommand Command;
	ESub_Command Sub_Command;
	int Room_ID;
	std::string Content;

};


struct FPost_Room_Packet
{
public:

	ECommand Command;
	ESub_Command Sub_Command;
	int Room_ID;
	std::string Room_Name;
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
	static FGet_Post_Packet Command_Packet_To_Get_Post_Packet(FCommand_Packet _com_packet);
	static std::string Get_Post_Packet_To_String(FGet_Post_Packet _get_packet);
	static FPost_Message_Packet Get_Post_Packet_To_Post_Message_Packet(FGet_Post_Packet _get_post_packet);
	static std::string Post_Message_Packet_To_String(FPost_Message_Packet __post_message_Packet);
	static FPost_Room_Packet Get_Post_Packet_To_Post_Room_Packet(FGet_Post_Packet _get_post_packet);
	static std::string Post_Room_Packet_To_String(FPost_Room_Packet _post_room_packet);
};

