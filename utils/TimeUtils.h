#ifndef LIBRARYMANAGEMENTSYSTEM_TIMEUTILS_H
#define LIBRARYMANAGEMENTSYSTEM_TIMEUTILS_H
#include <string>
#include <chrono>
using namespace std;

class TimeUtils {
public:
    static string formatTime(time_t timestamp);
};


#endif //LIBRARYMANAGEMENTSYSTEM_TIMEUTILS_H
