#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <fstream>

using namespace std;

int main ()
{   
    cout<<"Welcome , take 2 numbers , output their sum on first line , difference on second line "<<endl;
    while (1)
    {
        int sfd = socket(AF_INET , SOCK_STREAM , 0);
        if (sfd == -1)
        {
            cout<<"Error in creating socket "<<endl;
            return 0;
        }
        cout<<"Enter port : "<<endl;
        int port; cin>>port;
        struct sockaddr_in serveraddress;
        serveraddress.sin_family = AF_INET;
        serveraddress.sin_port = htons(port);
        serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
        if (t==-1)
        {
            cout<<"Error in connecting "<<endl;
            return 1;
        }
        cout<<"Enter file name : "<<endl;
        string file; cin>>file;
        ifstream fin(file);
        string line = ""; string word;

        while (getline(fin , line))
        {
            // cout<<line<<endl;
            word += line;
            word += '\n';
        }
        word += '\n';
        const char * buff = word.c_str();
        send(sfd , buff , strlen(buff)+1 , 0);
        if (shutdown(sfd, SHUT_WR) == -1) 
        {
            perror("shutdown() error");
        }
        sleep(2);

        char bux[100];
        recv(sfd , bux , 100 , 0);
        cout<<bux<<endl;
    }
}