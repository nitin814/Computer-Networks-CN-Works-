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

using namespace std;
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
    int usfd1=socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un servaddr;
    socklen_t servlen;
    servlen=sizeof(servaddr);
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sun_family=AF_UNIX;
    strcpy(servaddr.sun_path,"mysocket");

    int c = connect(usfd1,(struct sockaddr *)&servaddr,servlen);

    send(usfd1,"New Bowler Entered",strlen("New Bowler Entered"),0);

    int fd=recv_fd(usfd1);
    lseek(fd,0,SEEK_SET);

    int usfd2=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un servaddr1;
    socklen_t servlen1;
    servaddr1.sun_family=AF_UNIX;
    strcpy(servaddr1.sun_path,"mysocket1");
    servlen1=sizeof(servaddr1);
    unlink("mysocket1");
    bind(usfd2,(struct sockaddr *)&servaddr1,servlen1);
    listen(usfd2,10);
    struct sockaddr_in cliaddr;
    socklen_t clilen=sizeof(cliaddr);
    int nsfd=accept(usfd2,(struct sockaddr *)&cliaddr,&clilen);
    printf("Enter Speed and Spin values\n");
    char buff[100];
    fgets(buff,100,stdin);
    write(fd,buff,strlen(buff));
    send_fd(nsfd,fd);
    fd=recv_fd(nsfd);
    send_fd(usfd1,fd);
    
    return 0;
}