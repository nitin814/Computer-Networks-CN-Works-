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

    map<int,vector<int>> mp;

    while (1)
    {   
        struct mymsg* message1 = new mymsg();
        int result = msgrcv(msgqid1 , message1 , sizeof(message1->msgtext) , 0 , 0);
        
        if (result!=-1)
        {
            if (mp.find(message1->msgtype)==mp.end())
            {   
                cout<<"User with process "<<message1->msgtext<<" joined the chat "<<endl;
                
                vector<int> grpsin(26,0);
                for (int i=0;i<strlen(message1->msgtext);i++)
                {
                    for (int j=0;j<26;j++)
                    {
                        if (message1->msgtext[i]==('A'+j))
                            grpsin[j] = 1;
                    }
                }

                mp[(int)message1->msgtype] = grpsin;
                continue ;
            }
            cout<<"message recieved is : "<<message1->msgtext<<endl;
            char grp = message1->msgtext[0];
            for (auto j : mp)
            {
                if (j.first!=message1->msgtype)
                {
                    if ((j.second)[grp-'A']!=1)
                        continue;

                    // cout<<"Sending the message to "<<j.first<<endl;

                    struct mymsg message2;
                    message2.msgtype = j.first;
                    strncpy(message2.msgtext,message1->msgtext+2,strlen(message1->msgtext)-2);
                    message2.msgtext[strlen(message1->msgtext)] = '\0';

                    int isfine = msgsnd(msgqid2 , &message2 , strlen(message2.msgtext)+1 , 0);
                    
                    if (isfine==-1)
                    {
                        cout<<"Error in sending message "<<endl;
                        return 0;
                    }
                    // cout<<"Sent message .."<<endl;
                }
            }
        }
    }
}