#include <bits/stdc++.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <signal.h>
using namespace std;

struct message
{
    int type = -1;
    char buff[100];
};

struct data
{
    int lastindex;
    int lastindexread;
    struct message messagee[100];
};

struct data * res1 = NULL; 
struct data * res2 = NULL;

void func (int signo)
{
    int lastread = res1->lastindexread; int i = lastread+1;

    string text (res1->messagee[i].buff);
    int typee = res1->messagee[i].type;
    cout<<"got the text of "<<text<<" from pid of "<<typee<<endl;
    int in = res2->lastindex+1; res2->lastindex++;
    res2->messagee[in].type = typee;
    strcpy(res2->messagee[in].buff , text.c_str());
    (res2->messagee[in].buff)[strlen(res2->messagee[in].buff)] = '\0';

    res1->lastindexread++;
}

int main ()
{
    key_t key1 = ftok("./input.txt" , 'A');
    key_t key2 = ftok("./output.txt" , 'A');
    key_t key3 = ftok("./pidshare.txt" , 'A');

    signal (SIGUSR1 , func);

    if (key1==-1 || key2==-1 || key3==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }

    int shmid1 = shmget(key1 , 512 , IPC_CREAT | 0666);
    int shmid2 = shmget(key2 , 512 , IPC_CREAT | 0666);
    int shmid3 = shmget(key3 , 512 , IPC_CREAT | 0666);

    if (shmid1 == -1 || shmid2 == -1 || shmid3 == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }

    res1 = (struct data *)shmat(shmid1 , 0 , 0);
    res2 = (struct data *)shmat(shmid2 , 0 , 0);

    int * pid = (int *)shmat(shmid3 , 0 , 0);
    *pid = getpid();

    res1->lastindex = -1; res1->lastindexread = -1;
    res2->lastindex = -1;

    while (1)
    {  }

    int check = shmdt((void *)res1);

    if (check==-1)
    {
        cout<<"Detaching did not happen successfully "<<endl;
        return 0;
    }
}