#include "BorrowService.h"
#include <iostream>

// 定义静态成员变量
int BorrowService::MAX_BORROW_COUNT = 5;

BorrowService::BorrowService(BookCopyDAO *bookCopyDAO, RecordDAO *recordDAO, UserDAO *userDAO)
    : bookCopyDAO(bookCopyDAO), recordDAO(recordDAO), userDAO(userDAO) {
}

BorrowService::~BorrowService() = default;

//借阅书本
bool BorrowService::borrowBook(const string &userId, const string &copyId) const {
    if (recordDAO->addBorrowRecord(userId, copyId) && bookCopyDAO->updateBookCopyStatus(copyId, "borrowed") &&
        userDAO->updateUserBorrowInfo(userId, true)) {
        cout << "借阅成功" << endl; //显示在用户界面
        return true;
    }
    cout << "借阅失败" << endl; //显示在用户界面
    return false;
}

//归还书本
bool BorrowService::returnBook(const string &userId, const string &copyId) const {
    if (!hasUserBorrowedCopy(userId, copyId)) {
        cout << "你未借阅过该图书" << endl;
        return false;
    }
    if (recordDAO->updateReturnTime(userId, copyId) && bookCopyDAO->updateBookCopyStatus(copyId, "available") && userDAO
        ->updateUserBorrowInfo(userId, false)) {
        cout << "归还成功" << endl;
        return true;
    }
    cout << "归还失败" << endl;
    return false;
}

//用户是否可以继续借阅
bool BorrowService::canUserBorrowMore(const string &userId) const {
    int count = userDAO->getBorrowedBookCount(userId);;
    return count < MAX_BORROW_COUNT;
}

//获取用户当前借阅的记录
vector<Record> BorrowService::getUserBorrowingRecords(const string &userId) const {
    return recordDAO->getActiveRecordsByUser(userId);
}

//获取用户的借阅历史
vector<Record> BorrowService::getUserHistoryRecords(const string &userId) const {
    return recordDAO->getHistoryRecordsByUser(userId);
}

//书本是否已借出
bool BorrowService::isCopyBorrowed(const string &copyId) const {
    return bookCopyDAO->isCopyBorrowed(copyId);
}

//用户是否已借阅过该图书
bool BorrowService::hasUserBorrowedCopy(const string &userId, const string &copyId) const {
    return recordDAO->hasActiveRecordByUserId(userId, copyId);
}

// bool renewBook(const string& userId, const string& copyId);//续借
// bool hasOverdueBooks(const string& userId);//是否逾期

//设置最大借阅数量
void BorrowService::setMaxBorrowCount(int maxBorrowCount) {
    MAX_BORROW_COUNT = maxBorrowCount;
}
