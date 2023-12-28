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

    res1->lastindex = -1; res1->lastindexread = -1;
    res2->lastindex = -1;

    while (1)
    {
        int lastread = res1->lastindexread; int lastin = res1->lastindex;

        if (lastread < lastin)
        {   
            cout<<lastread<<" "<<lastin<<endl;
            for (int i = lastread+1 ; i<=lastin;i++)
            {
                string text (res1->messagee[i].buff);
                int typee = res1->messagee[i].type;
                cout<<"got the text of "<<text<<" from pid of "<<typee<<endl;
                int in = res2->lastindex+1; res2->lastindex++;
                res2->messagee[in].type = typee;
                strcpy(res2->messagee[in].buff , text.c_str());
                (res2->messagee[in].buff)[strlen(res2->messagee[in].buff)] = '\0';
            }
            res1->lastindexread = lastin;
        }
    }

    int check = shmdt((void *)res1);

    if (check==-1)
    {
        cout<<"Detaching did not happen successfully "<<endl;
        return 0;
    }
}