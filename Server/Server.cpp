
#include <iostream>
#include <cstring> // For memset
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // For close
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
#include <thread>

using namespace std;
class Server
{
private:
    int sockfd;
    sockaddr_in serverAddr, clientAddr;
    vector<sockaddr_in> clients;

    thread* receiveThread = nullptr;

public:
    Server(int port);
    ~Server();

    void sendToAll(char* msg, in_addr addr, int port);
    void receive();
    void broadcastMsg(char* message);
    
};

Server::Server(int port)
{
    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
    }


    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Listen on all available interfaces
    serverAddr.sin_port = htons(port);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);

    }
    std::cout << "Server is running on port " << port << std::endl;
    receiveThread = new thread([this]() { this->receive(); });
}

void Server::receive()
{

    //clients.push_back(clientAddr);

    char buffer[1024];
    socklen_t clientAddrLen = sizeof(clientAddr);

    while (true) {
        memset(buffer, 0, 1024);
        int bytesReceived = recvfrom(sockfd, buffer, 1024, 0,
                                    (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesReceived < 0) {
            perror("Receive failed");
            continue;
        }
        else
            clients.push_back(clientAddr);

        std::cout << "Received message: " << buffer << std::endl;

        broadcastMsg(buffer);
    }
}





void Server::broadcastMsg(char* message)
{
    for(auto client: clients)
    {
        in_addr addr =  client.sin_addr;
        int port = ntohs(client.sin_port);

        char *ip_str = inet_ntoa(addr);

        //Use ip_str as a C-style string
        std::cout << "IP address: " << ip_str << std::endl;

        sendToAll(message, addr, port);
    }

}

void Server::sendToAll(char* msg, in_addr addr, int port)
{
    sockaddr_in client;
    cout<<"port is=>"<<port;
    //broadcast msg
    // Message to send
    const char* message = msg;
    int messageLength = strlen(message);

    sockaddr_in ipaddr;
    // Configure server address
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr = addr;
    client.sin_port = htons(port);

    // Send the message
    int bytesSent = sendto(sockfd, message, messageLength, 0,
                        (struct sockaddr*)&client, sizeof(client));
    if (bytesSent < 0) {
        perror("Send failed");
        close(sockfd);
    }
}

Server::~Server()
{
    if (receiveThread) {
            receiveThread->join();
            delete receiveThread;
        }
}
