#include "services/BorrowService.h"

#include <ctime>
#include <iostream>

// 定义静态成员变量
int BorrowService::MAX_BORROW_COUNT = 5;

//借阅书本
bool BorrowService::borrowBook(const std::string &userId, const std::string &copyId) const {
    if (recordDAO->addBorrowRecord(userId, copyId) && bookCopyDAO->updateBookCopyStatus(copyId, "borrowed") &&
        userDAO->updateUserBorrowInfo(userId, true)) {
        return true;
    }
    return false;
}

//归还书本
bool BorrowService::returnBook(const std::string &userId, const std::string &copyId) const {
    if (recordDAO->updateReturnTime(userId, copyId) && bookCopyDAO->updateBookCopyStatus(copyId, "available") && userDAO
        ->updateUserBorrowInfo(userId, false)) {
        return true;
    }
    return false;
}

//用户是否可以继续借阅
bool BorrowService::canUserBorrowMore(const std::string &userId) const {
    int count = userDAO->getBorrowedBookCount(userId);;
    return count < MAX_BORROW_COUNT;
}

//获取用户当前借阅的记录
std::vector<Record> BorrowService::getUserBorrowingRecords(const std::string &userId) const {
    return recordDAO->getActiveRecordsByUser(userId);
}

//获取用户的借阅历史
std::vector<Record> BorrowService::getUserHistoryRecords(const std::string &userId) const {
    return recordDAO->getHistoryRecordsByUser(userId);
}

//获取当前已被借阅的图书数量
int BorrowService::getBorrowedBookCount() const {
    return bookCopyDAO->getBorrowedCopyCount();
}

//获取当前超期的图书数量
int BorrowService::getOverDueBookCount() const {
    std::vector<Record> records = recordDAO->getActiveRecords();
    int count = 0;
    for (const auto &record: records) {
        if (record.getDueTime() < time(nullptr)) {
            count++;
        }
    }
    return count;
}

//获取用户当前借阅的图书数量
int BorrowService::getCurrentBorrowCount(const std::string &userId) const {
    return userDAO->getBorrowedBookCount(userId);
}

//获取用户总借阅的图书数量
int BorrowService::getTotalBorrowCount(const std::string &userId) const {
    return static_cast<int>(recordDAO->getHistoryRecordsByUser(userId).size());
}

//获取用户即将到期的图书数量
int BorrowService::getDueSoonCount(const std::string &userId) const {
    const std::vector<Record> records = recordDAO->getActiveRecordsByUser(userId);
    int count = 0;
    for (const auto &record: records) {
        if (record.getDueTime() < time(nullptr) + 3 * 24 * 60 * 60) {
            count++;
        }
    }
    return count;
}

//获取用户超期的图书数量
int BorrowService::getOverdueCount(const std::string &userId) const {
    const std::vector<Record> records = recordDAO->getActiveRecordsByUser(userId);
    int count = 0;
    for (const auto &record: records) {
        if (record.getDueTime() < time(nullptr)) {
            count++;
        }
    }
    return count;
}

//书本是否已借出
bool BorrowService::isCopyBorrowed(const std::string &copyId) const {
    return bookCopyDAO->isCopyBorrowed(copyId);
}

//用户是否已借阅过该图书
bool BorrowService::hasUserBorrowedCopy(const std::string &userId, const std::string &copyId) const {
    return recordDAO->hasActiveRecordByUserId(userId, copyId);
}

// bool renewBook(const string& userId, const string& copyId);//续借
// bool hasOverdueBooks(const string& userId);//是否逾期

//设置最大借阅数量
void BorrowService::setMaxBorrowCount(const int maxBorrowCount) {
    MAX_BORROW_COUNT = maxBorrowCount;
}
