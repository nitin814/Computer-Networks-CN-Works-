#include <bits/stdc++.h>
#include <signal.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
using namespace std;

void fullcall (int signo)
{
    if (signo==SIGUSR1)
        cout<<"Got a signal from my parent . I am parent to child function "<<endl;
    else
        cout<<"Got a signal from my child . I am child to parent function "<<endl;
}


int main ()
{
    int t = 0; signal (SIGUSR1 , fullcall); signal (SIGUSR2 , fullcall);
    t = fork ();

    if (t==0)
    {
        cout<<"Child is getting executed "<<endl;
        kill (getppid() , SIGUSR2);
        sleep(4);
    }
    else
    {
        sleep(1);
        cout<<"Parent is getting executed "<<endl;
        kill (t , SIGUSR1);
    }
}