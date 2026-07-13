#include <iostream>

using namespace std;

int main()
{
     const int N = 10;

     int a[N];

     for(int i=0;i<N;i++)
          a[i]=0;

#pragma acc data copy(a)
     {

#pragma acc parallel loop present(a)
          for(int i=0;i<N;i++) {
               a[i]=i;
               cout<< a[i] <<endl;
          }

#pragma acc parallel loop present(a)
          for(int i=0;i<N;i++)
               a[i]*=10;

     }

     for(int i=0;i<N;i++)
          cout<<a[i]<<" ";

     cout<<endl;
}