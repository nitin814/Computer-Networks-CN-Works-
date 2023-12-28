#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>

using namespace std;

void func (int signalno , siginfo_t * info , void * context)
{
    cout<<"hello"<<endl;
    cout<<"Recieved a signal of "<<signalno<<" send from the pid of "<<info->si_pid<<endl;
}

int main ()
{
    struct sigaction sa;
    sa.sa_sigaction = func;
    sa.sa_flags = SA_SIGINFO; 
    sigemptyset(&sa.sa_mask);

    sigaction (SIGUSR1 , &sa , NULL);

    int c = 0;

    c = fork ();

    if (c==0)
    {
        cout<<"I am child with pid "<<getpid()<<endl;
        cout<<"Killing / sending signal to parent "<<endl;
        kill(getppid() , SIGUSR1);
        cout<<"DONe"<<endl;
    }
    else
    {
        wait(NULL);
        cout<<"done"<<endl;
    }
}