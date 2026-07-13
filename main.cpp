#include <iostream>
#include <openacc.h>

using namespace std;

class Test
{
public:
     int* arr;
     int N;

     Test(int n)
     {
          N = n;
          arr = new int[N];

          for(int i=0;i<N;i++)
               arr[i]=i;
     }

     ~Test()
     {
          delete[] arr;
     }

     void multiply()
     {
#pragma acc parallel loop present(this)
          for(int i=0;i<N;i++)
               arr[i]*=11;
     }
};

int main()
{
     Test t(10);

#pragma acc enter data copyin(&t)
#pragma acc enter data copyin(t.arr[0:t.N])

     acc_attach((void**)&t.arr);

     t.multiply();

#pragma acc update self(t.arr[0:t.N])

     for(int i=0;i<t.N;i++)
          cout<<t.arr[i]<<" ";

     cout<<endl;

#pragma acc exit data delete(t.arr[0:t.N])
#pragma acc exit data delete(&t)
}