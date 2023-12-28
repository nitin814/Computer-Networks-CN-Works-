#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>

using namespace std;

int main ()
{   
    string s = "Enter two numbers to add";
  
    const char * buf = s.c_str();
    write(1 , buf , strlen(buf)+1);
 
    char buffer[100];
    int n = read(0,buffer,100);
    buffer[n]='\0';

    stringstream ss(buffer);
    string num1,num2;
    ss>>num1>>num2;

    cout<<stoi(num1)*stoi(num2)<<endl;
    
}