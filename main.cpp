#include <iostream>

using namespace std;

int main()
{
     const int N = 10;

     int a[N];

     for(int i=0;i<N;i++)
          a[i]=-1;

#pragma acc enter data copyin(a)

#pragma acc parallel loop present(a)
     for(int i=0;i<N;i++)
          a[i]=i*10;

     // NEW
#pragma acc update self(a)

     for(int i=0;i<N;i++)
          cout<<a[i]<<" ";

     cout<<endl;

#pragma acc exit data delete(a)
}