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
    int key1 = ftok("./inputmsgb.txt" , 65);
    int key2 = ftok("./outputmsgb.txt" , 65);

    if (key1==-1 || key2==-1)
    {
        cout<<"Problem in generating valid key "<<endl;
    }

    int msgqid1 = msgget(key1 , IPC_CREAT | 0666);
    int msgqid2 = msgget(key2 , IPC_CREAT | 0666);

    map<int,int> mp;

    while (1)
    {   
        struct mymsg message1;
        int result = msgrcv(msgqid1 , &message1 , sizeof(message1.msgtext) , 0 , 0);
        
        if (result!=-1)
        {
            if (mp.find(message1.msgtype)==mp.end())
            {   
                cout<<"User with process "<<message1.msgtype<<" joined the chat "<<endl;
                mp[(int)message1.msgtype] = 1;
                continue ;
            }
            for (auto j : mp)
            {
                if (j.first!=message1.msgtype)
                {
                    cout<<"Sending the message to "<<j.first<<endl;

                    struct mymsg message2;
                    message2.msgtype = j.first;
                    strncpy(message2.msgtext,message1.msgtext,strlen(message1.msgtext));
                    message2.msgtext[strlen(message1.msgtext)] = '\0';

                    int isfine = msgsnd(msgqid2 , &message2 , strlen(message2.msgtext)+1 , 0);
                    
                    if (isfine==-1)
                    {
                        cout<<"Error in sending message "<<endl;
                        return 0;
                    }
                    cout<<"Sent message .."<<endl;
                }
            }
        }
    }
}