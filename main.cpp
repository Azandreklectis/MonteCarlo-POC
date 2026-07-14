#include <iostream>
using namespace std;

// #pragma acc routine seq
int square(int x)
{
    return x*x;
}

int main()
{
    const int N=10;

    int a[N];

#pragma acc parallel loop
    for(int i=0;i<N;i++)
        a[i]=square(i);

    for(int i=0;i<N;i++)
        cout<<a[i]<<" ";

    cout<<endl;
}