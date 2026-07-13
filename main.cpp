#include <iostream>

using namespace std;

int main()
{
     const int N = 100;

     int a[N];

#pragma acc parallel loop gang vector
     for(int i=0;i<N;i++)
          a[i]=i;

#pragma acc update self(a)

     for(int i=0;i<10;i++)
          cout<<a[i]<<" ";

     cout<<endl;
}