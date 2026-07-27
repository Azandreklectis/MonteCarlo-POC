#include "FileManager.h"

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

std::string FileManager::generateTimestamp()
{
    auto now = std::chrono::system_clock::now();

    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream stream;

    stream << std::put_time(&localTime, "%Y_%m_%d_%H%M%S");

    return stream.str();
}