#include "common/Logger.h"

#include <ctime>
#include <iomanip>
#include <sstream>

Logger::Logger() {
    accessFile.open("logs/access.log", std::ios::app);
    businessFile.open("logs/business.log", std::ios::app);
    errorFile.open("logs/error.log", std::ios::app);
}

std::string Logger::getCurrentTimestamp() {
    const time_t now = time(nullptr);
    const tm* timeInfo = localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::logAccess(const std::string &message) {
    std::lock_guard lock(logMutex);
    accessFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
}

void Logger::logBusiness(const std::string &message) {
    std::lock_guard lock(logMutex);
    businessFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
}

void Logger::logError(const std::string &message) {
    std::lock_guard lock(logMutex);
    errorFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
}
