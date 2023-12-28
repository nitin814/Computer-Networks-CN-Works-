#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main ()
{
	int tifd = open("./input.txt" , O_RDONLY);   // always make sure not to assign tifd to just an integer and do shit ..
	int tofd = open("./output.txt" , O_WRONLY);  // always make sure not to assign tofd to just an integer and do shit ..
	
	dup2(0 , tifd);
	dup2(1 , tofd);
	
	string str;
	
	int ifd = open("./input.txt" , O_RDONLY); 
	int ofd = open("./output.txt" , O_WRONLY);
	
	dup2 (ifd , 0);
	dup2 (ofd , 1);
	
	while (getline(cin , str))
	{
		cout<<str<<endl;
	}
	
	dup2(tifd , 0);
	dup2(tofd , 1);
	
	close(tifd); close(tofd);
	
	// cout<<tifd.is_open()<<endl;
	
	string str2;
	cin>>str2;
	cout<<str2<<endl;
	
	cout<<"naman"<<endl;
	
	cout<<"yes"<<endl;
	cout<<"no"<<endl;
}

