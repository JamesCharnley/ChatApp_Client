#include "MyApp.h"
#include "PacketDecoder.h"
#include "Client.h"

Client::Client(const char* _addressServer, USHORT _portServer, USHORT _portClient, MyApp* _main)
{
    main = _main;
    LocalPort = _portClient;
    ServerPort = _portServer;
    ServerAddress = _addressServer;
    InitWSA();
    CreateSocket(_portClient);
    BindSocket();
    std::thread clientThread(&Client::Connect, this);
    clientThread.detach();
    //Connect();
    //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "end of client constructor");
}

void Client::SendMessageToServer(std::string _message)
{
    size_t length = _message.length();
    char* buffer = new char[length + (size_t)1];
    for (int i = 0; i < (int)length; i++)
    {
        buffer[i] = _message[i];
    }
    buffer[length] = '\0';
    int status = send(Socket, buffer, (int)strlen(buffer), 0);
    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, buffer);
}

void Client::Connect()
{
    // create address for server connection
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(ServerPort);
    receiverAddr.sin_addr.s_addr = inet_addr(ServerAddress);

    // try connect to server
    int status = connect(Socket, (struct sockaddr*)&receiverAddr, sizeof(struct sockaddr));

    if (status == -1)
    {
        std::cout << "Error in connect(). Error code: " << WSAGetLastError() << "\n";
        return;
    }

    // pass connection to new thread
    std::thread sendThread(&Client::HandleConnection_Send, this, Socket, receiverAddr);
    std::thread recvThread(&Client::HandleConnection_Recv, this, Socket, receiverAddr);
    sendThread.join();
    recvThread.join();
    closesocket(Socket);
}

void Client::HandleConnection_Recv(int _socket, sockaddr_in _address)
{

    char buffer[BUFFER_SIZE];

    bool isActive = true;

    while (isActive)
    {

        int status = recv(_socket, buffer, BUFFER_SIZE, 0);

        if (status == -1)
        {
            std::cout << "ERROR in recv(). Error code: " << WSAGetLastError() << std::endl;
            isActive = false;
            continue;
        }
        buffer[(size_t)status] = '\0';

        ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("recv :"));
        ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, buffer);
        std::string msg(buffer);
        ProcessPacket(msg, (int)strlen(buffer));

        if (ClientShuttingDown)
        {
            isActive = false;
        }

    }
}

void Client::HandleConnection_Send(int _socket, sockaddr_in _address)
{

    char buffer[BUFFER_SIZE];

    bool isActive = true;

    while (isActive)
    {
        std::cout << "Enter a message to be sent: ";
        // gets_s DOES add a null terminator
        gets_s(buffer, BUFFER_SIZE);
        std::string str(buffer);
        if (str.length() < 1)
        {
            continue;
        }

        int status = send(_socket, buffer, (int)strlen(buffer) + 1, 0);

        if (status == -1)
        {
            std::cout << "Error in send().Error code: " << WSAGetLastError();
            isActive = false;
            continue;
        }

        std::string message(buffer);
        if (message == "QUIT")
        {
            ClientShuttingDown = true;
            closesocket(Socket);
        }

        if (ClientShuttingDown)
        {
            isActive = false;
        }
    }
}

void Client::ProcessPacket(std::string _packet, int _packet_length)
{
    FCommand_Packet com_packet = PacketDecoder::Char_To_Command_Packet(_packet, _packet_length);

    switch (com_packet.Command)
    {
    case ECommand::InValid:
        break;
    case ECommand::Login:
        break;
    case ECommand::Signup:
        break;
    case ECommand::Post:
        Post_Request(PacketDecoder::Command_Packet_To_Get_Post_Packet(com_packet));
        break;
    case ECommand::Get:
        break;
    case ECommand::Authorized:
        Login(com_packet);
        break;
    default:
        break;
    }
}

void Client::Login(FCommand_Packet _packet)
{
    FString_Packet p = PacketDecoder::Command_Packet_To_String_Packet(_packet);
    main->username = p._string;
    main->b_UserLoggedIn = true;
}

void Client::Post_Request(FGet_Post_Packet _packet)
{

    if (_packet.Sub_Command == ESub_Command::InValid)
    {
        ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("Post Request invalid"));
        return;
    }

    switch (_packet.Sub_Command)
    {
    case ESub_Command::InValid:
        break;
    case ESub_Command::Room:
        Post_Request_Room(PacketDecoder::Get_Post_Packet_To_Post_Room_Packet(_packet));
        break;
    case ESub_Command::RoomList:
        break;
    case ESub_Command::Contacts:
        break;
    case ESub_Command::Message:
        main->Post_Room_Meassage(PacketDecoder::Get_Post_Packet_To_Post_Message_Packet(_packet));
        break;
    default:
        break;
    }
    
}

void Client::Post_Request_Room(FPost_Room_Packet _packet)
{
    //main->AddRoom(_packet, main->get_db());
    main->post_room(_packet);
}
