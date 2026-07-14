#include <iostream>

using namespace std;

int main()
{
    const int N = 10000000;

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

// libstdc++.so.6 => /home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso/lib64/libstdc++.so.6 (0x00007f1e0aab7000)
// Bin 0 : 340
// Bin 1 : 340
// Bin 2 : 340
// Bin 3 : 340
// Bin 4 : 340
// Bin 5 : 340
// Bin 6 : 340
// Bin 7 : 340
// Bin 8 : 340
// Bin 9 : 340
//
// Total = 3400
