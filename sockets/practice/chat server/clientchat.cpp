#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/poll.h>
#include <unistd.h>

using namespace std;

int main (){
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1) perror("Socket error");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(55599);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connection
    int k = connect (sfd , (sockaddr *)&server_address , sizeof(server_address));
    if (k==-1) perror("Connection error");

    //----------------User Name Segment-------------------------
    cout<<"Enter name : ";
    string temp; getline(cin,temp);
    const char* name = temp.c_str();
    int t =send(sfd,name,strlen(name)+1,0);
    //-----------------------------------------------------------


    //poll on input from keyboard and sfd
    struct pollfd pfds[2];
    pfds[0].fd = 0;
    pfds[0].events = POLLIN;

    pfds[1].fd = sfd;
    pfds[1].events = POLLIN;


    while (1){
        int n = poll(pfds,2,1000);

        if(n==-1) perror("poll");
        
        else if(n==0) continue;

        else{
            for(int i=0; i<2; i++){
                if(i==0 && pfds[0].revents && POLLIN){
                    string s;
                    getline(cin,s);

                    const char* buffer = s.c_str();
                    int temp = send(sfd,buffer,strlen(buffer)+1,0);
                }
                else if(i==1 && pfds[1].revents && POLLIN){
                    char buffer[100];
                    int k = recv(sfd,buffer,100,0);
                    buffer[k]='\0';

                    cout<<buffer<<endl;
                }
            }
        }
    }
}