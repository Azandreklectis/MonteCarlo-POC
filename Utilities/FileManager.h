#pragma once

#include <string>

class FileManager
{
public:

    static std::string createRunDirectory();

private:

    static std::string generateTimestamp();
};