#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }

    cout<<"Enter port"<<endl;
    int port; cin>>port;
    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(port);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t len = sizeof(serveraddress);
    
    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }

    cout<<"enter word"<<endl;
    string str;
    cin>>str;
    const char * buf = str.c_str();
    send(sfd , buf , strlen(buf)+1 , 0);
    cout<<"done succesful"<<endl;
}