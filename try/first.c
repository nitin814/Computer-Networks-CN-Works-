#include <stdio.h>

int main ()
{
	int pid;
	printf("parent is --> %d and i am --> %d \n" , getppid() , getpid());
	
	pid = fork();
	
	if (pid==0)
	{
		printf("I am the child one with pid --> %d \n having parent as %d" , getpid() , getppid());
	} 
	else
	{
		printf("child will have a pid of --> %d with parent as %d \n" , pid , getpid());
	}
}
