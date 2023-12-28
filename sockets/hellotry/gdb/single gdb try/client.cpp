#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <fstream>

using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3001);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }

    ifstream fin("prog.cpp");
    string line = ""; string word;

    while (getline(fin , line))
    {
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
    // close(sfd);
    // cout<<"Enter the input : "<<endl;
    // string str1 , str2;
    // getline(cin , str1); getline(cin , str2); str1+='\0'; str2+='\0'; 
    // dup2(sfd , 1);

    // string pp = ""; pp += '\0';
    // cout<<pp<<endl;
    // cout<<str1<<endl; cout<<str2<<endl;

    // cout<<"writing succesfull"<<endl;
}