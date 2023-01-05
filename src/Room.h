#pragma once

#include <vector>
#include "Connection.h"
#include <string>

class Room
{

public:

	Room();

	Room(int _id, std::string _name);

	std::string GetName();
	int Get_ID() { return id; };

	void add_message(std::string _message);

	bool isInitialized = false;

	std::vector<std::string> messages = std::vector<std::string>();

private:

	std::string name = "";

	int id = 0;

	std::vector<std::string> members = std::vector<std::string>();

	

};

