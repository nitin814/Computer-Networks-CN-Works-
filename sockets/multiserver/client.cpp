#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main ()
{
    // serveraddress.sin_addr.s_addr = inet_addr("192.168.135.242");
    while (1)
    {   
        int sfd = socket(AF_INET , SOCK_STREAM , 0);
        if (sfd==-1)
        {
            cout<<"Error in creating socket"<<endl;
            return 1;
        }
        
        struct sockaddr_in serveraddress;
        serveraddress.sin_family = AF_INET;
        serveraddress.sin_port = htons(55553);
        serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
        if (t==-1)
        {
            cout<<"Error in connecting "<<endl;
            return 1;
        }

        cout<<"enTER CALL"<<endl;
        string str;
        getline(cin , str);
        const char * buf = str.c_str();
        int x = send(sfd , buf , strlen(buf)+1 , 0);
        
        char buffer[100];
        int n = recv(sfd,buffer,100,0);

        cout<<buffer<<endl;

        string s;
        getline(cin,s);
        buf= s.c_str();
        send(sfd,buf,strlen(buf)+1,0);
        cout<<"ok"<<endl;
        n = recv(sfd,buffer,100,0);
        buffer[n] = '\0';
        
        cout<<"Answer is : ";
        cout<<buffer<<endl;

        while (n = recv(sfd,buffer,100,0))
        {cout<<"shit"<<endl;}
    }
}