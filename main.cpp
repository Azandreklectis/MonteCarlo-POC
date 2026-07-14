#include <iostream>

using namespace std;

int main()
{
    const int N = 100000000;

    int *a = new int[N];

#pragma acc enter data create(a[0:N])

#pragma acc parallel loop present(a) async
    for(int i=0;i<N;i++)
        a[i]=i;

    cout<<"CPU is free"<<endl;

#pragma acc wait

#pragma acc update self(a[0:N])

    cout<<a[123]<<endl;

#pragma acc exit data delete(a[0:N])

    delete[] a;
}