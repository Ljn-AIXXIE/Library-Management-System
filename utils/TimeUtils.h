#ifndef LIBRARYMANAGEMENTSYSTEM_TIMEUTILS_H
#define LIBRARYMANAGEMENTSYSTEM_TIMEUTILS_H
#include <string>
#include <chrono>
using std::string;

class TimeUtils {
public:
    static std::string formatTime(time_t timestamp);
};


#endif //LIBRARYMANAGEMENTSYSTEM_TIMEUTILS_H
