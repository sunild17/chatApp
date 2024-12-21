#include<string>
#include<iostream>
#include "Client.cpp"

using namespace std;

class ClientWidow
{
private:
    /* data */
public:
    ClientWidow(string name, string ip, int port);
    ~ClientWidow();
};

ClientWidow::ClientWidow(string name, string ip, int port)
{
    Client obj(name, ip, port);
}

ClientWidow::~ClientWidow()
{
}
