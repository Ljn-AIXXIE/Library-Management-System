#include "Logger.h"
#include <ctime>
#include <iomanip>
#include <sstream>

Logger::Logger() {
    accessFile.open("logs/access.log", ios::app);
    businessFile.open("logs/business.log", ios::app);
    errorFile.open("logs/error.log", ios::app);
}

string Logger::getCurrentTimestamp() {
    const time_t now = time(nullptr);
    const tm *timeInfo = localtime(&now);
    ostringstream oss;
    oss << put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::logAccess(const string &message) {
    lock_guard lock(logMutex);
    accessFile << "[" << getCurrentTimestamp() << "] " << message << endl;
}

void Logger::logBusiness(const string &message) {
    lock_guard lock(logMutex);
    businessFile << "[" << getCurrentTimestamp() << "] " << message << endl;
}

void Logger::logError(const string &message) {
    lock_guard<mutex> lock(logMutex);
    errorFile << "[" << getCurrentTimestamp() << "] " << message << endl;
}
