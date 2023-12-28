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
    servaddr.sun_family=AF_UNIX;
    strcpy(servaddr.sun_path,"mysocket");
    servlen=sizeof(servaddr);
    unlink("mysocket");
    bind(usfd,(struct sockaddr *)&servaddr,servlen);
    listen(usfd,10);
    int rsfd=socket(AF_INET,SOCK_RAW,3);
    if(rsfd==-1)
    perror("socket");
    int nsfd;
    fd_set readfds;
    int fd=open("bowl.txt",O_CREAT|O_RDWR|O_TRUNC,0777);
    while(1)
    {
        struct sockaddr_in cliaddr;
        socklen_t clilen=sizeof(cliaddr);
        nsfd=accept(usfd,(struct sockaddr *)&cliaddr,&clilen);
        char buff[1000];
        int r=recv(nsfd,buff,1000,0);
        perror("recv ");
        buff[r]='\0';
        printf("%s\n",buff);
        send_fd(nsfd,fd);
        FD_SET(rsfd,&readfds);
        struct timeval tv={20,0};
        select(rsfd+1,&readfds,NULL,NULL,&tv);
        if(FD_ISSET(rsfd,&readfds))
        {
            char buf[1024];
            struct sockaddr_in client;
            socklen_t clilen=sizeof(client);
            printf("receive\n");
            recvfrom(rsfd,buf,1024,0,(struct sockaddr*)&client,(socklen_t*)&clilen);
            perror("recv");
            struct iphdr *ip;
            ip=(struct iphdr *)buf;
            printf("%s\n",buf+ip->ihl*4);
        }
        else
        {
            printf("The Batsman is Out\n");
        }
        fd=recv_fd(nsfd);
    }
    return 0;
}