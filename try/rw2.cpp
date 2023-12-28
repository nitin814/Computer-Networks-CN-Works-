#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main ()
{
	int ifd = open("./input.txt" , O_RDONLY);
	
	int ofd = open("./output.txt" , O_WRONLY);
	
	// dup2(idf , 0);
	
	char buffer[10];
	
	int n;
	while (n = read(ifd , buffer , 10))
	{	
		write(ofd , buffer , 10); 
	}
	
	close(ifd);
	close(ofd);
}

