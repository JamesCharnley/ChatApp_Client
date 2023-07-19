#include "Room.h"
#include <fstream>
#include <direct.h>
//#include <string.h>
#include <cstdint>
#include <filesystem>
#include "MyApp.h"
#include "Client.h"
#include "PacketDecoder.h"
Room::Room()
{
	
}

Room::Room(int _id, std::string _name, MyApp* _main, Client* _client)
{
	main = _main;
	client = _client;
	id = _id;
	name = _name;
	isInitialized = true;

	get_all_messages();
}

std::string Room::GetName()
{
	return name;
}

void Room::add_message(std::string _message)
{
	ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("Room::add_message()"));
	messages.push_back(_message);
}

void Room::get_all_messages()
{
	FGet_Post_Packet packet = { ECommand::Get, ESub_Command::Room, std::to_string(id) };
	client->SendMessageToServer(PacketDecoder::Get_Post_Packet_To_String(packet));
}



