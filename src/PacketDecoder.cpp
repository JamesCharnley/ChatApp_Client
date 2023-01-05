#include "PacketDecoder.h"

FCommand_Packet PacketDecoder::Char_To_Command_Packet(std::string _char, int _length)
{
    std::string toString = _char;

    int comLength = 0;
    bool isValid = false;
    for (int i = 0; i < toString.length(); i++)
    {
        if (toString[i] == ';')
        {
            isValid = true;
            break;
        }
        comLength++;
    }
    if (!isValid)
    {
        FCommand_Packet p = { ECommand::InValid, "" };
    }

    std::string com_str = toString.substr(0, (size_t)comLength);
    int com_int = std::stoi(com_str);
    ECommand com = (ECommand)com_int;

    std::string content_string = toString.substr((size_t)comLength + 1, toString.length() - ((size_t)comLength + 1));

    FCommand_Packet packet = { com, content_string };

    return packet;
}

std::string PacketDecoder::Command_Packet_To_String(FCommand_Packet _packet)
{
    std::string com_string = std::to_string((int)_packet.Command);
    std::string packet_string = com_string + ";" + _packet.Content;
    return packet_string;
}

FLogin_Packet PacketDecoder::Command_Packet_To_Login_Packet(FCommand_Packet _com_packet)
{
    int username_length = 0;
    for (int i = 0; i < _com_packet.Content.length(); i++)
    {
        if (_com_packet.Content[i] == ';')
        {
            break;
        }
        username_length++;
    }
    std::string username_sub = _com_packet.Content.substr(0, (size_t)username_length);

    //ultralight::String username(username_sub.c_str());

    int password_length = 0;
    for (int i = username_length + 1; i < _com_packet.Content.length(); i++)
    {
        if (_com_packet.Content[i] == ';')
        {
            break;
        }
        password_length++;
    }
    std::string password_sub = _com_packet.Content.substr((size_t)username_length + 1, (size_t)password_length);



    FLogin_Packet login_packet = { _com_packet.Command, username_sub, password_sub };


    return login_packet;
}

std::string PacketDecoder::Login_Packet_To_String(FLogin_Packet _login_packet)
{
    std::string com_string = std::to_string((int)_login_packet.Command);
    std::string packet_string = com_string + ";" + _login_packet.Username + ";" + _login_packet.Password;
    return packet_string;
}

FString_Packet PacketDecoder::Command_Packet_To_String_Packet(FCommand_Packet _com_packet)
{

    FString_Packet packet = { _com_packet.Command, _com_packet.Content };
    return packet;
}

std::string PacketDecoder::String_Packet_To_String(FString_Packet _string_packet)
{
    std::string com_string = std::to_string((int)_string_packet.Command);
    std::string packet_string = com_string + ";" + _string_packet._string;
    return packet_string;

}

FGet_Post_Packet PacketDecoder::Command_Packet_To_Get_Post_Packet(FCommand_Packet _com_packet)
{
    std::cout << "Command_Packet_To_Get_Packet" << std::endl;
    //std::cout << (int)_com_packet.Command << ";" << _com_packet.Content << std::endl;
    //FGet_Packet ppacket = { _com_packet.Command, ESub_Command::InValid, "" };
    //return ppacket;
    int sub_com_length = 0;
    for (int i = 0; i < _com_packet.Content.length(); i++)
    {
        if (_com_packet.Content[i] == ';')
        {
            break;
        }
        sub_com_length++;
    }
    if (sub_com_length == 0)
    {
        FGet_Post_Packet packet = { _com_packet.Command, ESub_Command::InValid, "" };
        std::cout << "return invalid" << std::endl;
        return packet;
    }

    std::string sub_com_string = _com_packet.Content.substr(0, sub_com_length);

    int sub_com_int = std::stoi(sub_com_string);
    ESub_Command sub_com = (ESub_Command)sub_com_int;

    int content_length = _com_packet.Content.length() - sub_com_length;
    if (content_length < 1)
    {
        FGet_Post_Packet packet = { _com_packet.Command, ESub_Command::InValid, "" };
    }

    std::string content_string = _com_packet.Content.substr(sub_com_length + 1, content_length);

    FGet_Post_Packet packet = { _com_packet.Command, sub_com, content_string };

    std::cout << "Return packet" << std::endl;
    return packet;
}

std::string PacketDecoder::Get_Post_Packet_To_String(FGet_Post_Packet _get_packet)
{
    std::string com_string = std::to_string((int)_get_packet.Command);
    std::string sub_com_string = std::to_string((int)_get_packet.Sub_Command);
    std::string packet_string = com_string + ";" + sub_com_string + ";" + _get_packet.Content;
    return packet_string;
}

FPost_Message_Packet PacketDecoder::Get_Post_Packet_To_Post_Message_Packet(FGet_Post_Packet _get_post_packet)
{

    int room_id_length = 0;
    bool inValid = true;
    for (int i = 0; i < _get_post_packet.Content.length(); i++)
    {
        if (_get_post_packet.Content[i] == ';')
        {
            inValid = false;
            break;
        }
        room_id_length++;
    }
    if (inValid)
    {
        FPost_Message_Packet inv_pack = { ECommand::InValid, ESub_Command::InValid, 0, "" };
        return inv_pack;
    }

    int room_id = std::stoi(_get_post_packet.Content.substr(0, room_id_length));
    std::string content = _get_post_packet.Content.substr(room_id_length + 1, _get_post_packet.Content.length() - room_id_length + 1);

    FPost_Message_Packet packet = { _get_post_packet.Command, _get_post_packet.Sub_Command, room_id, content };

    return packet;
}

std::string PacketDecoder::Post_Message_Packet_To_String(FPost_Message_Packet _post_message_Packet)
{
    int com_int = (int)_post_message_Packet.Command;
    std::string com_string = std::to_string(com_int);

    int sub_com_int = (int)_post_message_Packet.Sub_Command;
    std::string sub_com_string = std::to_string(sub_com_int);

    std::string packet_string = com_string + ";" + sub_com_string + ";" + std::to_string(_post_message_Packet.Room_ID) + ";" + _post_message_Packet.Content;
    return packet_string;
}

FPost_Room_Packet PacketDecoder::Get_Post_Packet_To_Post_Room_Packet(FGet_Post_Packet _get_post_packet)
{
    // 0;0;id;name

    int id_length = 0;
    for (int i = 0; i < _get_post_packet.Content.length(); i++)
    {
        if (_get_post_packet.Content[i] == ';')
        {
            break;
        }
        id_length++;
    }

    if (_get_post_packet.Content.length() <= id_length + 1)
    {
        // invalid packet
    }

    std::string id_string = _get_post_packet.Content.substr(0, id_length);
    int id_int = std::stoi(id_string);

    std::string name_string = _get_post_packet.Content.substr(id_length + 1, _get_post_packet.Content.length() - (id_length + 1));

    FPost_Room_Packet packet = { _get_post_packet.Command, _get_post_packet.Sub_Command, id_int, name_string };
    return packet;
}

std::string PacketDecoder::Post_Room_Packet_To_String(FPost_Room_Packet _post_room_packet)
{
    int com_int = (int)_post_room_packet.Command;
    std::string com_string = std::to_string(com_int);

    int sub_com_int = (int)_post_room_packet.Sub_Command;
    std::string sub_com_string = std::to_string(sub_com_int);

    std::string room_id_string = std::to_string(_post_room_packet.Room_ID);

    std::string packet_string = com_string + ";" + sub_com_string + ";" + room_id_string + ";" + _post_room_packet.Room_Name;

    return packet_string;
}
