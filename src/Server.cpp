#include "Server.h"

Server::Server(USHORT _port)
{

    InitWSA();
    CreateSocket(_port);
    BindSocket();
    // start a new thread for listening
    std::thread listenerThread(&Server::Listen, this);
    listenerThread.detach();
    // monitor the connections
    MonitorConnections();
}

void Server::ShutdownServer()
{
    // start shutting down server
    ServerShuttingDown = true;
    closesocket(Socket);
    
}

void Server::Listen()
{

    std::cout << "listening..\n";
    int status = listen(Socket, 5);

    if (status == -1)
    {
        std::cout << "Error in listen(). Error code: " << WSAGetLastError() << std::endl;
        //exit(-1);
        return;
    }

    int listenerSocket;
    sockaddr_in senderAddr;
    bool isActive = true;
    while (isActive)
    {
        int addrSize = sizeof(senderAddr);
        listenerSocket = accept(Socket, (struct sockaddr*)&senderAddr, &addrSize);

        if (listenerSocket == -1)
        {
            if (ServerShuttingDown)
            {
                std::cout << "'QUIT' Command received - Server is shutting down\n";
                isActive = false;
            }
            else
            {
                std::cout << "Error in accept(). Error code: " << WSAGetLastError() << std::endl;
                std::cout << "Accepting next connection...\n\n";
            }
            continue;
        }

        std::cout << "A connection from " << inet_ntoa(senderAddr.sin_addr) << " has been accepted\n";

        // create a connection class and pass it the socket
        Connection* newConnection = new Connection(this, listenerSocket);

        // lock the newconnections vector untill it is empty and add new connection
        std::unique_lock<std::mutex>lock(Queue_Mutex);
        Queue_cv.wait(lock, [this] { return NewConnectionQueue.empty(); });
        NewConnectionQueue.push_back(newConnection);
        lock.unlock();
        
    }
    
    closesocket(listenerSocket);
}


void Server::MonitorConnections()
{
    bool isActive = true;
    while (isActive)
    {
        // lock newconnections while looping through all new connections and adding them to Connections
        std::lock_guard<std::mutex>lock(Queue_Mutex);
        int length = NewConnectionQueue.size();
        for (int i = 0; i < length; i++)
        {
            std::vector<Connection*>::iterator it_new = NewConnectionQueue.begin() + i;
            if (it_new != NewConnectionQueue.end())
            {
                Connection* newCon = *it_new;
                Connections.push_back(newCon);
            }
        }
        NewConnectionQueue.clear();

        // unlock newconnections vector
        Queue_cv.notify_one();

        // loop through all connections and check their status
        length = Connections.size();
        for (int i = 0; i < length; i++)
        {
            std::vector<Connection*>::iterator it = Connections.begin() + i;
            if (it != Connections.end())
            {
                Connection* con = *it;
                // connection has finished closing so delete the pointer and remove from vector
                if (con->IsConnectionClosed())
                {
                    delete *it;
                    Connections.erase(it);
                    break;
                }
                else if (ServerShuttingDown)
                {
                    // start closing connection
                    con->CloseConnection();
                }
                
            }
            
        }
        if (ServerShuttingDown && Connections.size() == 0)
        {
            isActive = false;
        }
        
    }
}
