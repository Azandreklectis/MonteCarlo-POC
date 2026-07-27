#include <iostream>

#include "Dataset/DatasetGenerator.h"

using namespace std;

int main()
{
    try
    {
        DatasetGenerator generator;

        generator.generate();

        return 0;
    }
    catch (const exception& e)
    {
        cerr << endl;
        cerr << "========== EXCEPTION ==========" << endl;
        cerr << e.what() << endl;
        cerr << "===============================" << endl;

        return -1;
    }
    catch (...)
    {
        cerr << endl;
        cerr << "Unknown exception caught!" << endl;

        return -2;
    }
}