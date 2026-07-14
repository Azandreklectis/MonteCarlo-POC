#include <iostream>

using namespace std;

int main()
{
    const int N = 10;

    int hist[10];

    for(int i=0;i<10;i++)
        hist[i]=0;

#pragma acc parallel loop
    for(int i=0;i<N;i++)
    {
        int bin = i % 10;      // deterministic instead of random
        hist[bin]++;
    }

    int total = 0;

    for(int i=0;i<10;i++)
    {
        cout << "Bin " << i << " : " << hist[i] << endl;
        total += hist[i];
    }

    cout << "\nTotal = " << total << endl;
}