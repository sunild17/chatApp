#include<iostream>
#include<string>
#include "Server.cpp"

using namespace std;




class ServerMain
{
private:
    /* data */
public:
    ServerMain(int port);
    ~ServerMain();
};

ServerMain::ServerMain(int port)
{
    Server obj(port);
}

ServerMain::~ServerMain()
{
}


int main(int argc, char** argv)
{
    int port  = 12345;
    if(argc != 1)
    {
         cout<<"usage==> ./server <#portnum> "<<endl;
         return 0;
    }
    else
       new ServerMain(port); 
}
