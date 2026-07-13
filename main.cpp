#include <iostream>

using namespace std;

int main()
{
    const int N = 100;

    int sum = 0;

#pragma acc parallel loop
    for(int i=0;i<N;i++)
        sum++;

    cout << sum << endl;
}