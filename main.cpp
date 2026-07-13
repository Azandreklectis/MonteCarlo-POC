#include <iostream>

using namespace std;

int main()
{
     const int N = 100;

     int a[N];

#pragma acc enter data create(a)

#pragma acc parallel loop gang vector
     for(int i=0;i<N;i++)
          a[i]=i;

#pragma acc update self(a)
#pragma acc exit data delete(a)

     for(int i=0;i<10;i++)
          cout<<a[i]<<" ";

     cout<<endl;
}

// 9, Generating enter data create(a[:])
//    Generating NVIDIA GPU code
//    14, #pragma acc loop gang, vector(96) /* blockIdx.x threadIdx.x */
// 9, Generating implicit copyout(a[:]) [if not already present]
//
// 15, Generating update self(a[:])
//    Generating exit data delete(a[:])