#include "common/Logger.h"

#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace {

std::filesystem::path getLogsRootDirectory() {
    namespace fs = std::filesystem;
    if (const char *envDir = std::getenv("LIBRARY_LOG_DIR")) {
        if (envDir[0] != '\0') {
            return fs::path(envDir);
        }
    }
#ifdef LIBRARY_PROJECT_ROOT_STR
    return fs::path(LIBRARY_PROJECT_ROOT_STR) / "logs";
#else
    return fs::current_path() / "logs";
#endif
}

void openDailyLogFile(std::ofstream &out, const std::filesystem::path &filePath) {
    namespace fs = std::filesystem;
    const fs::path parent = filePath.parent_path();
    if (!parent.empty()) {
        fs::create_directories(parent);
    }
    if (!fs::exists(filePath)) {
        std::ofstream create(filePath);
    }
    out.clear();
    out.open(filePath, std::ios::app);
}

} // namespace

Logger::Logger() = default;

std::string Logger::getCurrentTimestamp() {
    const time_t now = time(nullptr);
    const tm *timeInfo = localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::getCurrentDateYmd() {
    const time_t now = time(nullptr);
    const tm *timeInfo = localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d");
    return oss.str();
}

void Logger::ensureFilesForCurrentDay() {
    const std::string day = getCurrentDateYmd();
    if (day == currentLogDay && accessFile.is_open() && businessFile.is_open() && errorFile.is_open()) {
        return;
    }
    currentLogDay = day;
    accessFile.close();
    businessFile.close();
    errorFile.close();
    namespace fs = std::filesystem;
    const fs::path base = getLogsRootDirectory();
    const fs::path fileName = day + ".log";
    openDailyLogFile(accessFile, base / "access" / fileName);
    openDailyLogFile(businessFile, base / "business" / fileName);
    openDailyLogFile(errorFile, base / "error" / fileName);
}

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::logAccess(const std::string &message) {
    std::lock_guard<std::mutex> lock(logMutex);
    ensureFilesForCurrentDay();
    if (accessFile.is_open()) {
        accessFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
    }
}

void Logger::logBusiness(const std::string &message) {
    std::lock_guard<std::mutex> lock(logMutex);
    ensureFilesForCurrentDay();
    if (businessFile.is_open()) {
        businessFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
    }
}

void Logger::logError(const std::string &message) {
    std::lock_guard<std::mutex> lock(logMutex);
    ensureFilesForCurrentDay();
    if (errorFile.is_open()) {
        errorFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
    }
}
