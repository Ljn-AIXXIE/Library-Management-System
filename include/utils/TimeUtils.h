#ifndef LIBRARY_MANAGEMENT_SYSTEM_TIMEUTILS_H
#define LIBRARY_MANAGEMENT_SYSTEM_TIMEUTILS_H

#include <string>
#include <chrono>

class TimeUtils {
public:
    static std::string formatTime(const time_t timestamp) {
        char buffer[20];
        const tm *timeInfo = std::localtime(&timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeInfo);
        return std::string(buffer);
    }

    static time_t stringToTimeT(const std::string &timeStr, const std::string &format = "%Y-%m-%d") {
        std::tm tm = {};
        std::istringstream ss(timeStr);
        ss >> std::get_time(&tm, format.c_str());
        return std::mktime(&tm);
    }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_TIMEUTILS_H
