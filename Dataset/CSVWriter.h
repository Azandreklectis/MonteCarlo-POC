#pragma once

#include <fstream>
#include <string>

#include "DatasetRecord.h"

using namespace std;

class CSVWriter
{
public:

    explicit CSVWriter(const string& filename);

    ~CSVWriter();

    void writeHeader();

    void append(const DatasetRecord& record);

private:

    ofstream file;
};