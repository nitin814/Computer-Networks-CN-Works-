#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/poll.h>
#include <unistd.h>

using namespace std;

int main (){

    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1) perror("Socket error");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(55599);
    address.sin_addr.s_addr = INADDR_ANY;

    //Bind
    int a = bind (sfd , (struct sockaddr *)&address , sizeof(address));
    if (a == -1) perror("Bind error");

    //Listen
    int b = listen(sfd , 10);
    if (b == -1) perror("Listen error");

    cout<<"Listening"<<endl;

    //poll
    struct pollfd pfds[10];
    pfds[0].fd = sfd;
    pfds[0].events = POLLIN;
    int curr=1;

    //to keep track to all connected clients
    vector<int> connected_clients;
    unordered_map<int,string> mp;

    while(1){
        int n = poll(pfds,curr,1000);

        if(n==-1) perror("poll");
        
        else if(n==0) continue;
        
        else{
            for(int i=0; i<curr; i++){
                if(i==0){
                    //if a new client comes, we accept and store it's fd and also poll  
                    if(pfds[0].revents && POLLIN){
                        struct sockaddr * client_address;
                        socklen_t len = sizeof(client_address);

                        int nsfd = accept(sfd , (struct sockaddr*)&client_address , &len);
                        connected_clients.push_back(nsfd);

                        pfds[curr].fd = nsfd;
                        pfds[curr].events = POLLIN;
                        curr++;

                        //We also read name and store it
                        char buffer[100];
                        int k = recv(nsfd,buffer,100,0);
                        buffer[k]='\0';
                        mp[nsfd]=buffer;
                    }
                }
                else{
                    if(pfds[i].revents && POLLIN){
                        //someone has sent something, so we receive first and then sent it others

                        char buffer[100];
                        int k = recv(pfds[i].fd, buffer,100,0);
                        buffer[k]='\0';

                        //now s contains name of sender
                        char send_msg[300]; int j=0;
                        for(j; buffer[j]!='\0'; j++){
                            send_msg[j]=buffer[j];
                        }
                        send_msg[j++]= ' '; send_msg[j++]= ':'; send_msg[j++]= ' ';
                        string temp = mp[pfds[i].fd];
                        for(int k=0; k<temp.length(); k++){
                            send_msg[j]=temp[k];
                            j++; 
                        }
                        send_msg[j]='\0';

                        // send it to rest
                        for(int j=0; j<connected_clients.size(); j++){
                            if(connected_clients[j]!=pfds[i].fd){
                                send(connected_clients[j],send_msg,strlen(send_msg)+1,0);
                            }
                        }
                    }
                }
            }
        }
    }
}