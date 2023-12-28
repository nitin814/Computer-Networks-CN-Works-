#include<time.h>
#include<stdio.h>
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
int send_fd(int socket, int fd_to_send)
{
	struct msghdr socket_message;
	struct iovec io_vector[1];
	struct cmsghdr *control_message = NULL;
	char message_buffer[1];
	char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
	int available_ancillary_element_buffer_space;
	message_buffer[0] = 'F';
	io_vector[0].iov_base = message_buffer;
	io_vector[0].iov_len = 1;
	memset(&socket_message, 0, sizeof(struct msghdr));
	socket_message.msg_iov = io_vector;
	socket_message.msg_iovlen = 1;
	available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
	memset(ancillary_element_buffer, 0,
		   available_ancillary_element_buffer_space);
	socket_message.msg_control = ancillary_element_buffer;
	socket_message.msg_controllen =
		available_ancillary_element_buffer_space;
	control_message = CMSG_FIRSTHDR(&socket_message);
	control_message->cmsg_level = SOL_SOCKET;
	control_message->cmsg_type = SCM_RIGHTS;
	control_message->cmsg_len = CMSG_LEN(sizeof(int));
	*((int *)CMSG_DATA(control_message)) = fd_to_send;
	return sendmsg(socket, &socket_message, 0);
}
int recv_fd(int socket)
{
	int sent_fd, available_ancillary_element_buffer_space;
	struct msghdr socket_message;
	struct iovec io_vector[1];
	struct cmsghdr *control_message = NULL;
	char message_buffer[1];
	char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
	memset(&socket_message, 0, sizeof(struct msghdr));
	memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));
	io_vector[0].iov_base = message_buffer;
	io_vector[0].iov_len = 1;
	socket_message.msg_iov = io_vector;
	socket_message.msg_iovlen = 1;
	socket_message.msg_control = ancillary_element_buffer;
	socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
	if (recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
		return -1;
	if (message_buffer[0] != 'F')
	{
		return -1;
	}
	if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
	{
		return -1;
	}
	for (control_message = CMSG_FIRSTHDR(&socket_message);
		 control_message != NULL;
		 control_message = CMSG_NXTHDR(&socket_message, control_message))
	{
		if ((control_message->cmsg_level == SOL_SOCKET) &&
			(control_message->cmsg_type == SCM_RIGHTS))
		{
			sent_fd = *((int *)CMSG_DATA(control_message));
			return sent_fd;
		}
	}
	return -1;
}
int main()
{
    int usfd;
    usfd=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un servaddr;
    socklen_t servlen;
    servlen=sizeof(servaddr);
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sun_family=AF_UNIX;
    strcpy(servaddr.sun_path,"mysocket1");
    int c=connect(usfd,(struct sockaddr *)&servaddr,servlen);
    int fd=recv_fd(usfd);
    lseek(fd,0,SEEK_SET);
    char buff[1000];
    int r=read(fd,buff,1000);
    buff[r]='\0';
    char x[10];
    int j=0;
    int speed=0,spin=0;
    for(int i=0;buff[i]!='\0';i++)
    {
        if(buff[i]==' ')
        {
            x[j]='\0';
            speed=atoi(x);
            j=0;
            continue;
        }
        x[j]=buff[i];
        j++;
    }
    x[j]='\0';
    spin=atoi(x);
    printf("%d %d\n",speed,spin);
    srand(time(0));
    int k=(rand()%(40-0+1))+0;
    int rsfd=socket(AF_INET,SOCK_RAW,2);
	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	printf("opt\n");
	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");
	char buff1[10];
	client.sin_addr.s_addr=INADDR_ANY;
	unsigned int client_len=sizeof(client);
    sprintf(buff1,"%d",k);
	printf("sending\n");
	sendto(rsfd,buff1,strlen(buff1),0,(struct sockaddr*)&client,sizeof(client));
    send_fd(usfd,fd);
    return 0;
}