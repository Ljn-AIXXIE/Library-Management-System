#ifndef LIBRARY_MANAGEMENT_SYSTEM_LOGGER_H
#define LIBRARY_MANAGEMENT_SYSTEM_LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger {
    Logger();

    std::ofstream accessFile;
    std::ofstream businessFile;
    std::ofstream errorFile;
    std::mutex logMutex;

    static std::string getCurrentTimestamp();

public:
    static Logger &getInstance();

    void logAccess(const std::string &message);
    void logBusiness(const std::string &message);
    void logError(const std::string &message);
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_LOGGER_H