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
#include <sys/types.h>
#include <signal.h>
using namespace std;

int step = 1;  int pidofp2 = -1;
int * data1 = NULL; int * data2 = NULL;

void func (int signo)
{
    cout<<pidofp2<<" has sent an increment . "<<endl;
    cout<<"recieved value : "<<*data2<<endl;
    *data1 = *data2 + 1;
}

int main ()
{
    signal (SIGUSR1 , func);
    key_t key1 = ftok("./input1.txt" , 'A');
    key_t key2 = ftok("./input2.txt" , 'A');

    if (key1==-1 || key2==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }

    int shmid1 = shmget(key1 , 512 , IPC_CREAT | 0666);
    int shmid2 = shmget(key2 , 512 , IPC_CREAT | 0666);
   
    if (shmid1 == -1 || shmid2 == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }

    data2 = (int *)shmat(shmid1 , 0 , 0);
    data1 = (int *)shmat(shmid2 , 0 , 0);
    *data1 = getpid();

    while (*data2==-1)
    {
        cout<<"ok"<<endl;
        sleep(2);
    }
    cout<<*data1<<" "<<*data2<<endl;
    pidofp2 = *data2;
    *data2 = 0;

    sleep(3);

    while (1)
    {
        sleep(1);
        kill (pidofp2 , SIGUSR2);
        pause();
    }
}