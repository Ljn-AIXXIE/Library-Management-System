#include "TimeUtils.h"

//辅助函数：返回时间的字符串表示
string TimeUtils::formatTime(time_t timestamp) {
    char buffer[20];

    tm *timeInfo = std::localtime(&timestamp);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeInfo);

    return string(buffer);
}
