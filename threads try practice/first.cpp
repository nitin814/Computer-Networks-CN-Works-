#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

void * printhello (void * args)
{
    sleep(2);
    cout<<"Hello 1"<<endl;
    return ((void *)0);
}

void * printhello2 (void * args)
{
    cout<<"Hello 2"<<endl;
    return ((void *)0);
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
    pthread_join(coutone , NULL);
    sleep(2);
    return 0;
}