#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int main()
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serv_addr;
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(3000);
	
	connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	
	while (1)
	{
		string str; cin>>str;
		const char * buf = str.c_str();
		send(sfd,buf,strlen(buf),0);
	}
	return 0;
}

