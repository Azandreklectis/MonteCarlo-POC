#include "SpinWriter.h"

#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

string SpinWriter::save(
    const string& folder,
    int runID,
    const int* spinData,
    int totalSites)
{
    stringstream filename;

    filename
        << folder
        << "/spins/run_"
        << setw(4)
        << setfill('0')
        << runID
        << ".bin";

    ofstream file(
        filename.str(),
        ios::binary);

    if (!file.is_open())
    {
        return "";
    }

    file.write(
        reinterpret_cast<const char*>(spinData),
        totalSites * sizeof(int));

    file.close();

    stringstream relative;

    relative
        << "spins/run_"
        << setw(4)
        << setfill('0')
        << runID
        << ".bin";

    return relative.str();

}