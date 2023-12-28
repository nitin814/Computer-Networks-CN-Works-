#include <bits/stdc++.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>

using namespace std;

int cnt = 0;
void sigchld_handler(int signum) 
{
    int status;
    pid_t child_pid;

    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) 
    {
        if (WIFEXITED(status)) 
        {
            cnt++; cout << "Child process " << child_pid << " terminated with exit status"<<endl;
        } 
        else if (WIFSIGNALED(status)) 
        {
            cnt++; cout << "Child process " << child_pid << " terminated by signal "<<endl;
        }
    }
}

int main() 
{

    signal(SIGCHLD, sigchld_handler);

    for (int i = 0; i < 5; ++i) 
    {
        pid_t child_pid = fork();

        if (child_pid == 0) 
        {
            sleep(5+2*i); 
            return 0;
        }
    }
    
    while (cnt != 5) 
    {
        cout<<"Parent process is running."<<endl;
        sleep(1);
    }
    cout<<"All childs dones , now i also terminating "<<endl;

    return 0;
}
