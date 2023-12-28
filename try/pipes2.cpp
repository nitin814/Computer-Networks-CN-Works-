#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main ()
{
	int pp[2];
	int r = pipe(pp);
	
	int ifd = open("./input.txt" , O_RDONLY);
	
	if (r<0)
		return 0;
	
	int c = fork();
	
	if (c>0) // parent code
	{
		close(pp[0]);
		
		char buffer [100];
		while (read(ifd , buffer , 100))
		{
			write(pp[1] , buffer , 100);
		}
	}
	else
	{
		close(pp[1]);
		
		dup2(pp[0] , 0);
		
		close(pp[0]);
		
		char * args[] = {NULL};
		execv("./one" , args);
		
		cout<<"wont get executed"<<endl;
	}
}
