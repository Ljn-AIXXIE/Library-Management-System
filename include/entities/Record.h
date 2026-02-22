#ifndef LIBRARY_MANAGEMENT_SYSTEM_RECORD_H
#define LIBRARY_MANAGEMENT_SYSTEM_RECORD_H

#include <string>
#include <utility>

class Record {
    std::string userId;
    std::string copyId;
    time_t borrowTime;
    time_t returnTime;
    time_t dueTime;

    static constexpr int DAYS_TO_KEEP = 30;  // 借阅保留天数

public:
    Record() = default;
    // 构造函数（仅借出时间）
    Record(std::string userId, std::string copyId, const time_t borrowTime)
        : userId(std::move(userId)), copyId(std::move(copyId)), borrowTime(borrowTime), returnTime(0),
          dueTime(borrowTime + DAYS_TO_KEEP * 24 * 60 * 60) {}

    // 构造函数（包含归还时间）
    Record(std::string userId, std::string copyId, const time_t borrowTime, const time_t returnTime)
        : userId(std::move(userId)), copyId(std::move(copyId)), borrowTime(borrowTime), returnTime(returnTime),
          dueTime(borrowTime + DAYS_TO_KEEP * 24 * 60 * 60) {}

    [[nodiscard]] const std::string& getUserId() const { return userId; }
    [[nodiscard]] const std::string& getCopyId() const { return copyId; }
    [[nodiscard]] time_t getBorrowTime() const { return borrowTime; }
    [[nodiscard]] time_t getReturnTime() const { return returnTime; }
    [[nodiscard]] time_t getDueTime() const { return dueTime; }

    void setUserId(const std::string &newUserId) { userId = newUserId; }
    void setCopyId(const std::string &newCopyId) { copyId = newCopyId; }
    void setBorrowTime(time_t newBorrowTime) { borrowTime = newBorrowTime; }
    void setReturnTime(time_t newReturnTime) { returnTime = newReturnTime; }
    void setDueTime(time_t newDueTime) { dueTime = newDueTime; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_RECORD_H