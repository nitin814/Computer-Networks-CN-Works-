#include <bits/stdc++.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <pthread.h>

using namespace std;

vector<int> arr;

void merge (int low ,int mid , int high)
{
    vector<int> newarr;
    int one = low; int two = mid+1;
    while (one<=mid && two<=high)
    {
        if (arr[one]<=arr[two])
        {
            newarr.push_back(arr[one]);
            one++;
        }
        else
        {
            newarr.push_back(arr[two]);
            two++;
        }
    }

    while (one<=mid)
    {
        newarr.push_back(arr[one]);
        one++;
    }
    while (two<=high)
    {
        newarr.push_back(arr[two]);
        two++;
    }

    for (int i = low;i<=high;i++)
    {
        arr[i] = newarr[i - low];
    }
}

void mergesort (int low , int high)
{
    if (low>=high)
        return;

    int mid = (low+high)/2;

    mergesort (low , mid);
    mergesort (mid+1 , high);

    merge (low ,mid , high);
}

void * func (void * args)
{
    int in = *((int *)args);
    cout<<"I am thread "<<in<<endl;
    in = (in*(arr.size()))/4;
    int lastin = min (arr.size()-1 , in + arr.size()/4 - 1);

    cout<<"I am thread above , calling from "<<in<<" to "<<lastin<<endl;
    mergesort (in , lastin);

    return NULL;
}

void mergenew (int low,  int high)
{
    if (low>=high)
        return ;
    
    int mid = (low+high)/2;
    pthread_t one,two;
    pthread_create(&one , NULL , mergenew , )
}

int main ()
{
    int n; cin>>n;
    arr.resize(n);
    cout<<"size of arr is : "<<arr.size()<<endl;
    for (int i=0;i<n;i++)
        cin>>arr[i];
    
    mergenew (0 , n-1);
    // pthread_t vals [4];
    // int pp[4];
    // for (int i=0;i<4;i++)
    // {
    //     cout<<i<<endl;
    //     pp[i] = i;
    //     int s = pthread_create(&vals[i] , NULL , func , (void *)&pp[i]);
    //     if (s==-1)
    //     {
    //         cout<<"Error in creating "<<endl;
    //         return 0;
    //     }
    // }

    // for (int i=0;i<4;i++)
    //     pthread_join(vals[i] , NULL);
    
    for (int i=0;i<n;i++)
        cout<<arr[i]<<" ";
}