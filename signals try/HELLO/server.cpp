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

int main ()
{
    int p1,p2,p3;
    cout<<"Enter the pids of all the processes : "<<endl;
    cin>>p1>>p2>>p3;
    string t="kill -CONT "+to_string(p1) ;
    system(t.c_str()) ;
    t="kill -CONT "+to_string(p2) ;
    system(t.c_str()) ;
    t="kill -CONT "+to_string(p3) ;
    system(t.c_str()) ;

	int gpid;
	printf("Enter GPID Number to send signal : ");
	cin>>gpid;

	killpg(gpid, SIGUSR1) ;
}