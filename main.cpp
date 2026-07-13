#include <iostream>

using namespace std;

int main()
{
     const int N = 100;

int a[N][N];

#pragma acc parallel loop collapse(2)
for(int i=0;i<N;i++)
{
    for(int j=0;j<N;j++)
    {
        a[i][j] = i + j;
    }
}
}