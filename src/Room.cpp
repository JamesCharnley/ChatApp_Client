#include "Room.h"
#include <fstream>
#include <direct.h>
//#include <string.h>
#include <cstdint>
#include <filesystem>

Room::Room()
{
	
}

Room::Room(int _id, std::string _name)
{
	id = _id;
	name = _name;
	isInitialized = true;
}

std::string Room::GetName()
{
	return name;
}

void Room::add_message(std::string _message)
{
	messages.push_back(_message);
}

//
//void Room::RemoveActiveUser(Connection& _userConnection)
//{
//	std::vector<Connection&>::iterator it;
//	for (int i = 0; i < activeUsers.size(); i++)
//	{
//		it = activeUsers.begin() + i;
//		Connection& con = *it;
//
//		if (_userConnection.GetUsername() == con.GetUsername())
//		{
//			activeUsers.erase(it);
//			return;
//		}
//	}
//}
//
//void Room::NewMessage(std::string _message)
//{
//}

