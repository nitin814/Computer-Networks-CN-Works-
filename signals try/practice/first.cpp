#include <bits/stdc++.h>
#include <signal.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
using namespace std;

void ptoc (int signo)
{
    cout<<"Got a signal from my parent . I am parent to child function "<<endl;
}

void ctop (int signo)
{
    cout<<"Got a signal from my child . I am child to parent function "<<endl;
}

int main ()
{
    int t = 0; signal (SIGUSR1 , ptoc); signal (SIGUSR2 , ctop);
    t = fork ();

    if (t==0)
    {
        cout<<"Child is getting executed "<<endl;
        kill (getppid() , SIGUSR2);
        sleep(100);
        cout<<"OK bye"<<endl;
    } 
    else
    {
        // wait(NULL);
        sleep(50);
        cout<<"Parent is getting executed "<<endl;
        kill (t , SIGUSR1);
    }
}