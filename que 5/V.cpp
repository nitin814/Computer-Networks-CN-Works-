#include <bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <poll.h>
#include <sys/types.h>

using namespace std;

int main ()
{   
    // creating raw socket
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9998);
    // change the ip as per vehicle id number 
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // sending connection request to D
    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }


    string str;
    cout<<"What do you want ? petrol / disel / gas : "<<endl;
    cin>>str;
    // sending request of petrol/disel to D
    send(sfd , str.c_str() , str.length() , 0);

    sleep(2);
    string f = "20";
    // sending the billing amount 
    send(sfd , f.c_str() , f.length() , 0);

    while (1)
    {
        // recieving the message when either petrol/disel/gas is not available
        char buff[100];
        recv(sfd , buff , 100 , 0);
        cout<<"Received : "<<buff<<endl;
    }
}