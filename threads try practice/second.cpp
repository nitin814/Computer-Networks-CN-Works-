#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

struct values
{
    int start;
    int count;
};

int value = 0;
void * sum1 (void * args)
{
    struct values * real = ((struct values *)args);
    int sum = 0;
    for (int i=(real->start);i<(real->count);i++)
    {
        sum += i;
    }
    value += sum;
    pthread_exit ((void *)(intptr_t)sum);
    // return (void *)(intptr_t)sum;
}

int main ()
{
    pthread_t countone;
    pthread_t counttwo;
    struct values forone,fortwo;
    forone.start = 0; fortwo.start = 0;
    forone.count = 5; fortwo.count = 6;
    int summ1 = 0; int summ2 = 0;

    int s = pthread_create(&countone , NULL , sum1 , (void *)&forone);
    if (s!=0)
    {
        cout<<"Thread not created successfully "<<endl;
        return 0;
    }

    int d = pthread_create(&counttwo , NULL , sum1 , (void *)&fortwo);
    if (d!=0)
    {
        cout<<"Thread not created successfully "<<endl;
        return 0;
    }

    pthread_join(countone , (void**)&summ1);
    pthread_join(counttwo , (void**)&summ2);

    cout<<summ1<<" "<<summ2<<endl;

    cout<<value<<endl;
    return 0;
}