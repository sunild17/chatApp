
#include<string>
#include<iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define BUFFER_SIZE 1024

using namespace std;

class Client
{
private:
    string name;
    int port;
    string ip;

    int sockfd;
    sockaddr_in serverAddr;
    thread* t_receive;
    thread* t_send;
public:
    Client(string name, string ip, int port);
    ~Client();
    bool ClientopenConnection(string address);

    void send(string msg);
    void receive();
};

Client::Client(string name, string ip, int port)
{
    this->name = name;
    this->ip = ip;
    this->port = port;

    ClientopenConnection(ip);
    string msg = "";
    cout <<"enter message to send"<<endl;
    cin >> msg;
    send(msg);
    receive();
}

bool Client::ClientopenConnection(string address) {
    const char* ip = address.c_str();
	// Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    
	return true;
}

void Client::send(string msg)
{   
    // Message to send
    const char* message = msg.c_str();
    int messageLength = strlen(message);

    // Send the message
    int bytesSent = sendto(sockfd, message, messageLength, 0,
                           (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bytesSent < 0) {
        perror("Send failed");
        close(sockfd);
    }
}
void Client::receive()
{
    char * buffer[1024];
    socklen_t serverAddrLen = sizeof(serverAddr);
    memset(buffer, 0, 1024);
    int bytesReceived = recvfrom(sockfd, buffer, 1024, 0,
                                (struct sockaddr*)&serverAddr, &serverAddrLen);
    if (bytesReceived < 0) {
        perror("Receive failed");
    }

    std::cout << "Received message is: " << buffer << std::endl;
    
}

Client::~Client()
{
    // t_send->join();
    // t_receive->join(); 
    close(sockfd);
}
