#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main ()
{
	// close(0);
	
	// int ifd = open("./input.txt" , O_RDONLY);
	
	// close(1);
	
	// int ofd = open("./output.txt" , O_WRONLY);
	
	// cout<<ifd<<"  "<<ofd<<endl;
	
	// // dup2(idf , 0);
	
	// string str;
	
	// while (getline(cin , str))
	// {
	// 	cout<<str<<endl;
	// }
	
	int ifd = open("./input.txt", O_RDONLY);

	dup2(0 , ifd);
	cout<<ifd<<"naman"<<endl;
	string str;
	
	char buffer [1024] = {0};

	while (read(ifd , buffer , 1024))
	{
		cout<<buffer<<endl;
		cout<<endl;
	}
}

