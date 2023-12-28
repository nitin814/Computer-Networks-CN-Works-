#include <bits/stdc++.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>

using namespace std;

struct mymsg 
{
    long msgtype;
    char msgtext[512];
};

int main ()
{
    int key1 = ftok("./inputmsg.txt" , 65);
    int key2 = ftok("./outputmsg.txt" , 65);

    if (key1==-1 || key2==-1)
    {
        cout<<"Problem in generating valid key "<<endl;
    }

    int msgqid1 = msgget(key1 , IPC_CREAT | 0666);
    int msgqid2 = msgget(key2 , IPC_CREAT | 0666);

    // cout<<msgqid1<<"  "<<msgqid2<<endl;
    long pid = (long)getpid();

    struct mymsg initials;

    initials.msgtype = 1;

    char const* pidd = (to_string(getpid())).c_str();

    strncpy(initials.msgtext,pidd,strlen(pidd));

    initials.msgtext[strlen(pidd)] = '\0';

    int isfine = msgsnd(msgqid1 , &initials , strlen(initials.msgtext)+1 , 0);

    if (isfine==-1)
    {
        cout<<"Error in sending message "<<endl;
        return 0;
    }

    struct pollfd pfd[1];
    pfd[0].fd = 0; pfd[0].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , 1 , 100);

        if (s>0 && pfd[0].revents & POLLIN)
        {
            cout<<"Sending the message "<<endl;
            struct mymsg message1;
            fgets(message1.msgtext , 500 , stdin);
            message1.msgtype = pid;
            message1.msgtext[strlen(message1.msgtext)] = '\0';
            // cout<<message1.msgtext<<endl;

            int isfine = msgsnd(msgqid1 , &message1 , strlen(message1.msgtext)+1 , 0);
    
            if (isfine==-1)
            {
                cout<<"Error in sending message "<<endl;
                return 0;
            }
        }

        struct mymsg message2;
        int result = msgrcv(msgqid2 , &message2 , sizeof(message2.msgtext) , getpid() , MSG_NOERROR | IPC_NOWAIT);
        
        if (result!=-1)
        {
            // cout<<strlen(message2.msgtext)<<endl;
            cout<<"new message : "<<message2.msgtext<<endl;
        }
    }
}