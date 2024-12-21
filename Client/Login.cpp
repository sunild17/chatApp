#include<string>
#include<iostream>
#include "ClientWidow.cpp"

using namespace std;

class Login {

    public:
        void login()
        {
            cout <<"enter name:"<<"\n";
            cin >> usrname;

            cout <<"ip add default is : localhost"<<"\n";
            ipadd = "127.0.0.1";

            cout <<"port default is : 12345"<<"\n";
            port = 12345;

            new  ClientWidow(usrname, ipadd, port);


        }


    private:
    string ipadd;
    int port;
    string usrname;
};


int main()
{
    Login obj;
    obj.login();
}
