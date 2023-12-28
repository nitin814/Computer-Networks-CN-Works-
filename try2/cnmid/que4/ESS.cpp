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
#include <sys/shm.h>
using namespace std;

int main ()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8001);
    server.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(sfd, (struct sockaddr *)&server, sizeof(server));
    if (bind_status == -1)
    {
        cout << "Bind Error" << endl;
        exit(0);
    }

    int listen_status = listen(sfd, 5);
    if (listen_status == -1)
    {
        cout << "Listen Error" << endl;
        exit(0);
    }

    int nsfd = accept(sfd, NULL, NULL);
    cout<<"accepted "<<endl;

    while (1)
    {
        char buf[100];
        int r = recv(nsfd, buf, sizeof(buf)-1, 0);
        buf[r] = '\0';

        cout<<"read the thang "<<buf<<endl;
    }
}