#include <bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <csignal>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <pthread.h>

using namespace std;

int main ()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connect_status = connect(sfd, (struct sockaddr *)&addr, sizeof(addr));
    if (connect_status == -1)
    {
        cout<<"connect error"<<endl;
        return 0;
    }

    while (1)
    {
        string str;
        cin>>str;
        send(sfd, str.c_str(), str.length(), 0);
        cout<<"sended "<<endl;
    }
}