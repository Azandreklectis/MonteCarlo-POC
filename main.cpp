#include <iostream>

using namespace std;

int main()
{
     cout << "MEOWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" <<endl;
     const int N = 10;

     int a[N];

#pragma acc kernels
     for (int i = 0; i < N; i++)
     {
          a[i] = i * 10;
     }

     for (int i = 0; i < N; i++)
     {
          cout << a[i] << " ";
     }

     cout << endl;

     return 0;
}