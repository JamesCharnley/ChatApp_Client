#include "PacketDecoder.h"
#include "MyApp.h"
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

FString_Packet PacketDecoder::Command_Packet_To_String_Packet(FCommand_Packet _com_packet)
{

    FString_Packet packet = { _com_packet.Command, _com_packet.Content };
    return packet;
}
