#include <iostream>
#include <openacc.h>

using namespace std;

int main()
{
     const int N = 16;

     int a[N];

#pragma acc parallel loop
     for (int i = 0; i < N; i++)
     {
          a[i] = i * i;
     }

     cout << "Array:\n";

     for (int i = 0; i < N; i++)
     {
          cout << a[i] << " ";
     }

     cout << endl;

     return 0;
}