#include <bits/stdc++.h>
#include <sys/shm.h>
using namespace std;

struct message
{
    int type = -1;
    char buff[100];
};

struct data
{
    int count = 0;
    struct message messagee[100];
};

int main ()
{
    key_t key = ftok("./input.txt" , 'A');

    if (key==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }

    int shmid = shmget(key , 512 , IPC_CREAT | 0666);
   
    if (shmid == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }
    
    struct data * res = (struct data *)shmat(shmid , 0 , 0);
    res->count = 0;
    res->count++;
    for (int i=0;i<100;i++)
    {
        if (res->messagee[i].type!=88)
        {
            res->messagee[i].type = 88;
            strcpy(res->messagee[i].buff , "send this to p1 and p2");
            res->messagee[i].buff[strlen(res->messagee[i].buff)] = '\0';
            break;
        }
    }

    int check = shmdt((void *)res);

    if (check==-1)
    {
        cout<<"Detaching did not happen successfully "<<endl;
        return 0;
    }
    cout<<"yo";
}