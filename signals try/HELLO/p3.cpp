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

void func (int signo)
{
    cout<<"called by leader"<<endl;
}

int main ()
{
    signal(SIGUSR1 , func);
    cout<<"My pid is : "<<getpid()<<endl;
    int mypid = (int)getpid();
    int setgroupid;
    cout<<"Enter group id : "<<endl;
    cin>>setgroupid;

    setpgid(mypid , setgroupid);
    
    string t="kill -STOP "+to_string(getpid()) ;

    system(t.c_str());

    sleep(2);
    cout<<"Now having gpid of "<<getpgid(mypid)<<endl;
	while(1)
    {
		sleep(1);
		
	}
}