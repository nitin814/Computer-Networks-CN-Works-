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

int main ()
{
    key_t key1 = ftok("./input.txt" , 'A');
    key_t key2 = ftok("./output.txt" , 'A');

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

    struct data * res1 = (struct data *)shmat(shmid1 , 0 , 0);
    struct data * res2 = (struct data *)shmat(shmid2 , 0 , 0);

    struct pollfd pfd[1]; 
    pfd[0].fd = 0; pfd[0].events = POLLIN;

    int lastreadin = -1;

    sem_t* mutex;
    const char* mutex_name = "/savetxt"; 

    mutex = sem_open(mutex_name, O_CREAT, 0644, 1);
    
    if (mutex == SEM_FAILED) 
    {
        cout<<"error for mutex , wasted"<<endl;
        return 0;
    }

    while (1)
    {
        int s = poll(pfd , 1 , 300);
        
        if (s>0 && (pfd[0].revents & POLLIN))
        {   
            int pp = sem_wait(mutex);
            if (pp==-1)
            { cout<<"mutex error"<<endl; return 0; }

            string line;
            getline(cin , line);
            int x = res1->lastindex + 1; res1->lastindex++;

            // sleep(5);    if we do this .. it will increment the index .. also the server will read from the latest incremented index which is not yet overwritten . so it will read garbage value or previous put value ..
            // also there are chances that both the clients increment x at the same time , thus overwriting it ... and leaving the next index as empty ... to prevent all this , i think syncronisation is required ...
            
            res1->messagee[x].type = getpid();
            strcpy(res1->messagee[x].buff , line.c_str());
            (res1->messagee[x].buff)[line.length()] = '\0';

            pp = sem_post(mutex);
            if (pp==-1)
            { cout<<"mutex error"<<endl; return 0; }
        }

        int p = res2->lastindex;
        if (lastreadin < p)
        {
            for (int i = lastreadin+1;i<=p;i++)
            {
                if (res2->messagee[i].type != getpid())
                {
                    string textmess (res2->messagee[i].buff);
                    cout<<"New message : "<<textmess<<endl;
                }
            }
            lastreadin = p;
        }
    }

    int check = shmdt((void *)res1);

    if (check==-1)
    {
        cout<<"Detaching did not happen successfully "<<endl;
        return 0;
    }
}