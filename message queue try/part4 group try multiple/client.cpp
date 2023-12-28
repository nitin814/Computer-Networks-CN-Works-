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
    char msgtext[512] = {'\0'};
};

int main ()
{
    long pid = (long)getpid();

    cout<<"Enter in what all groups do you want to be : (A/B/C) "<<endl;
    string grp; 
    getline(cin,grp); vector<int> msgqidin(3 , -1); vector<int> msgqidout(3 , -1);

    for (int i=0;i<grp.length();i++)
    {
        if (grp[i]=='A')
        {
            int key1 = ftok("./inputmsga.txt" , 65);
            msgqidin[0] = msgget(key1 , IPC_CREAT | 0666);
            int key2 = ftok("./outputmsga.txt" , 65);
            msgqidout[0] = msgget(key2 , IPC_CREAT | 0666);
        }
        if (grp[i]=='B')
        {
            int key1 = ftok("./inputmsgb.txt" , 65);
            msgqidin[1] = msgget(key1 , IPC_CREAT | 0666);
            int key2 = ftok("./outputmsgb.txt" , 65);
            msgqidout[1] = msgget(key2 , IPC_CREAT | 0666);
        }
        if (grp[i]=='C')
        {
            int key1 = ftok("./inputmsgc.txt" , 65);
            msgqidin[2] = msgget(key1 , IPC_CREAT | 0666);
            int key2 = ftok("./outputmsgc.txt" , 65);
            msgqidout[2] = msgget(key2 , IPC_CREAT | 0666);
        }
    }

    for (int i=0;i<3;i++)
    {
        if (msgqidin[i]==-1)
            continue;
        
        struct mymsg initials;
        initials.msgtype = pid;
        initials.msgtext[0] = 'J';
        initials.msgtext[1] = '\0';

        int isfine = msgsnd(msgqidin[i] , &initials , 2 , 0);
        if (isfine==-1)
        {
            cout<<"Error in sending message "<<endl;
            return 0;
        }
    }
    
    struct pollfd pfd[1];
    pfd[0].fd = 0; pfd[0].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , 1 , 100);

        if (s>0 && pfd[0].revents & POLLIN)
        {
            struct mymsg message1;
            fgets(message1.msgtext , 100 , stdin);

            if (grp.find(message1.msgtext[0])==string::npos)
            {
                cout<<"Sorry , you dont belong to this group "<<endl;
                continue;
            }

            message1.msgtype = pid;
            message1.msgtext[strlen(message1.msgtext)] = '\0';
            cout<<"Sending the message "<<endl;

            for (int i=0;i<3;i++)
            {
                if (message1.msgtext[0]==('A'+i))
                {
                    int isfine = msgsnd(msgqidin[i] , &message1 , strlen(message1.msgtext)+1 , 0);
    
                    if (isfine==-1)
                    {
                        cout<<"Error in sending message "<<endl;
                        return 0;
                    }
                }
            }
        }

        for (int i=0;i<3;i++)
        {
            if (msgqidout[i]!=-1)
            {
                struct mymsg message2;
                int result = msgrcv(msgqidout[i] , &message2 , sizeof(message2.msgtext) , getpid() , MSG_NOERROR | IPC_NOWAIT);
                
                if (result!=-1)
                    cout<<"new message : "<<message2.msgtext<<endl;
            }
        }
    }
}