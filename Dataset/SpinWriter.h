
#ifndef OPENACC_TEST_SPINWRITER_H
#define OPENACC_TEST_SPINWRITER_H

#endif //OPENACC_TEST_SPINWRITER_H
#pragma once

#include <string>

using namespace std;

class SpinWriter
{
public:

    static string save(
        const string& folder,
        int runID,
        const int* spinData,
        int totalSites
    );
};