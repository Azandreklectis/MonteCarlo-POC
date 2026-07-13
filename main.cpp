#include <iostream>

using namespace std;

int main()
{
     int a[10];

     for (int i = 0; i < 10; i++)
          a[i] = 5;

#pragma acc data copyout(a)
     {
#pragma acc parallel loop
          for (int i = 0; i < 10; i++)
          {
               a[i] += 1;
          }
     }

     for (int i = 0; i < 10; i++)
          cout << a[i] << " ";
}