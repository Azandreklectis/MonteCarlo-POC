#include <iostream>

using namespace std;

int main()
{
     const int N = 10;

     int* a = new int[N];

     for(int i = 0; i < N; i++)
          a[i] = i;

     // Allocate device memory for the array and copy it
#pragma acc enter data copyin(a[0:N])

     // Use the array already present on the GPU
#pragma acc parallel loop present(a)
     for(int i = 0; i < N; i++)
     {
          a[i] *= 10;
     }

     // Bring results back
#pragma acc update self(a[0:N])

     for(int i = 0; i < N; i++)
          cout << a[i] << " ";

     cout << endl;

     // Free GPU memory
#pragma acc exit data delete(a[0:N])

     delete[] a;
}