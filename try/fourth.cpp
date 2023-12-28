#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main ()
{
	int c = 0;
	int ifd = open("./input.txt" , O_RDONLY);
	
	c = fork();
	
	if (c>0)
	{
	}
	else
	{
		dup2(ifd , 0);
		char * args[] = {NULL};
		int f = execv("./one" , args);
		cout<<"NOOOO"<<endl;
	}
}
