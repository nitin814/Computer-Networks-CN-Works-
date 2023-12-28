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
    int key = ftok("./chat.txt" , 'A');
    if (key==-1)
    {
        cout<<"Problem in generating valid key "<<endl;
    }

    int msgqid = msgget(key , 0666);

    struct mymsg message1;

    int result = msgrcv(msgqid , &message1 , sizeof(message1.msgtext) , 1 , MSG_NOERROR);

    if (result==-1)
        cout<<"failed to get message"<<endl;
    else
        cout<<message1.msgtext<<endl;
}