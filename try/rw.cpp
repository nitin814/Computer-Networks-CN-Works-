#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main ()
{
	close(0);
	int ifd = open("./input.txt" , O_RDONLY);
	dup(ifd);
	
	// dup2(idf , 0);
	
	cout<<ifd<<endl;
	char buffer[257];
	
	string str;
	while (getline(cin,str))
	{
		cout<<str<<endl;
	}
}
