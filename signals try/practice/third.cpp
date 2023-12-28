#include <bits/stdc++.h>
#include <signal.h>

using namespace std;

#define ll long long

void func (int signo)
{
    cout<<"HAHAAHA! "<<endl;
}
void func2 (int signo)
{
    cout<<"yoyoy"<<endl;
}
int main ()
{
    signal(SIGINT , func);

    signal(SIGINT , func2);

    while (1)
    {
        cout<<"Will not terminate ! "<<endl;
        // raise(SIGINT);
        kill (getpid() , SIGINT);
        sleep(1);
    }
}
