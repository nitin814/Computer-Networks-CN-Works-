#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

void * printhello (void * args)
{
    while (1)
    {
        string str;
        cin>>str;
        cout<<str<<endl;
    }
}

void * printhello2 (void * args)
{
    while (1)
    {
        cout<<"Enter a text ... "<<endl;
        sleep(2);
    }
}

int main ()
{
    pthread_t coutone;
    pthread_t couttwo;
    int s = pthread_create(&coutone , NULL , printhello , NULL);
    if (s!=0)
    {
        cout<<"Thread not created successfully "<<endl;
        return 0;
    }

    int d = pthread_create(&couttwo , NULL , printhello2 , NULL);
    if (d!=0)
    {
        cout<<"Thread not created successfully "<<endl;
        return 0;
    }

    pthread_join(coutone , NULL);
    pthread_join(couttwo , NULL);
    sleep(2);
    return 0;
}