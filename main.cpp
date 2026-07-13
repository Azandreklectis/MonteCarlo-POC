#include <iostream>

using namespace std;

int main()
{
    const int N = 100;

    int sum = 0;

#pragma acc data copy(sum)
    {
#pragma acc parallel loop present(sum)
        for(int i=0;i<N;i++)
        {
            sum++;
        }

#pragma acc update self(sum)
    }

    cout << sum << endl;
}