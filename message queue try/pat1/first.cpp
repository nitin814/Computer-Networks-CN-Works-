#include <bits/stdc++.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

using namespace std;

struct mymsg 
{
    long msgtype;
    char msgtext[512];
};

int main ()
{
    key_t key = ftok("./chat.txt" , 65);
    if (key==-1)
    {
        cout<<"Problem in generating valid key "<<endl;
    }

    int msgqid = msgget(key , IPC_CREAT | 0666);

    long pid = (long)getpid();

    struct mymsg initials;

    initials.msgtype = 1;

    char const* pidd = (to_string(getpid())).c_str();


    strncpy(initials.msgtext,pidd,strlen(pidd));

    initials.msgtext[strlen(pidd)] = '\0';

    // cout<<pidd<<"  "<<strlen(initials.msgtext)<<endl;

    int isfine = msgsnd(msgqid , &initials , strlen(initials.msgtext) , 0);

    // cout<<isfine<<endl;

    if (isfine==-1)
    {
        cout<<"Error in sending message "<<endl;
        return 0;
    }

    // struct mymsg message1;

    // message1.msgtype = 1;
    // fgets(message1.msgtext , sizeof(message1.msgtext) , stdin);
    // // strcpy(message1.msgtext , "Hello , i am hello");

    // int isfine = msgsnd(msgqid , &message1 , sizeof(message1.msgtext) , 0);
    
    // if (isfine==-1)
    // {
    //     cout<<"Error in sending message "<<endl;
    //     return 0;
    // }
}   