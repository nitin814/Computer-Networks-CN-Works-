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
#include <pthread.h>

using namespace std;

struct mymsg 
{
    long msgtype;
    char msgtext[512];
};

void * reads (void * args)
{
    int msgqid1 = *((int *)(args));
    long pid = (long)getpid();
    while (1)
    {
        struct mymsg message1;
        fgets(message1.msgtext , 500 , stdin);
        message1.msgtype = pid;
        message1.msgtext[strlen(message1.msgtext)] = '\0';

        int isfine = msgsnd(msgqid1 , &message1 , strlen(message1.msgtext)+1 , 0);
        if (isfine==-1)
        {
            cout<<"Error in sending message "<<endl;
            return ((void *)(intptr_t)0);
        }
        cout<<"Sending the message "<<endl;
    }
    return ((void *)(intptr_t)0);
}

void * writes (void * args)
{
    int msgqid2 = *((int *)(args));
    while (1)
    {
        struct mymsg message2;
        int result = msgrcv(msgqid2 , &message2 , sizeof(message2.msgtext) , getpid() , MSG_NOERROR | IPC_NOWAIT);
        
        if (result!=-1)
        {
            cout<<"new message : "<<message2.msgtext<<endl;
        }
        sleep(0.5);
    }
}

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

    pthread_t reading;
    pthread_t writing;

    int s = pthread_create(&reading , NULL , reads , ((void *)&msgqid1));
    if (s!=0)
    {
        cout<<"Thread not created successfully "<<endl;
        return 0;
    }

    int d = pthread_create(&writing , NULL , writes , ((void *)&msgqid2));
    if (d!=0)
    {
        cout<<"Thread not created successfully "<<endl;
        return 0;
    }

    pthread_join(reading , NULL);
    pthread_join(writing , NULL);
}