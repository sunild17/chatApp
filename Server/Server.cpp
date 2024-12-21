
#include <iostream>
#include <cstring> // For memset
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // For close
#include <arpa/inet.h>
#include <vector>
#include <algorithm>

using namespace std;
class Server
{
private:
    int sockfd;
    sockaddr_in serverAddr, clientAddr;
    vector<sockaddr_in> clients;

public:
    Server(int port);
    ~Server();


    void receive()
    {

        clients.push_back(clientAddr);



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

            std::cout << "Received message: " << buffer << std::endl;

            manageClients(buffer);
        }
    }

    void manageClients(char* message)
    {
        for(auto client: clients)
        {
            in_addr addr =  client.sin_addr;

            char *ip_str = inet_ntoa(addr);

            //Use ip_str as a C-style string
            std::cout << "IP address: " << ip_str << std::endl;

            send(message, addr);
       }

    }

    void send(char* msg, in_addr addr)
    {
        //broadcast msg
        // Message to send
        const char* message = msg;
        int messageLength = strlen(message);

        sockaddr_in ipaddr;
        // Configure server address
        memset(&serverAddr, 0, sizeof(ipaddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr = addr;
        //serverAddr.sin_port = htons(SERVER_PORT);

        // Send the message
        int bytesSent = sendto(sockfd, message, messageLength, 0,
                            (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent < 0) {
            perror("Send failed");
            close(sockfd);
        }
    }
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
    receive();
}

Server::~Server()
{
}
