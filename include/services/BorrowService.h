#ifndef LIBRARY_MANAGEMENT_SYSTEM_BORROWSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_BORROWSERVICE_H

#include "database/dao/BookCopyDAO.h"
#include "database/dao/RecordDAO.h"
#include "database/dao/UserDAO.h"

class BorrowService {
    BookCopyDAO *bookCopyDAO;
    RecordDAO *recordDAO;
    UserDAO *userDAO;
    static int MAX_BORROW_COUNT;
public:
    BorrowService(BookCopyDAO *bookCopyDAO, RecordDAO *recordDAO, UserDAO *userDAO)
        : bookCopyDAO(bookCopyDAO), recordDAO(recordDAO), userDAO(userDAO) {}

    // 核心业务
    [[nodiscard]] bool borrowBook(const std::string &userId, const std::string &copyId) const; //借阅书本
    [[nodiscard]] bool returnBook(const std::string &userId, const std::string &copyId) const; //归还书本

    // 业务校验
    [[nodiscard]] bool canUserBorrowMore(const std::string &userId) const; //用户是否可以继续借阅
    [[nodiscard]] bool isCopyBorrowed(const std::string &copyId) const; //书本是否已借出
    [[nodiscard]] bool hasUserBorrowedCopy(const std::string &userId, const std::string &copyId) const; //用户是否有借阅该书本
    //bool hasOverdueBooks(const string& userId);//是否逾期

    // 查询类
    [[nodiscard]] std::vector<Record> getUserBorrowingRecords(const std::string &userId) const; //获取用户当前借阅的记录
    [[nodiscard]] std::vector<Record> getUserHistoryRecords(const std::string &userId) const; //获取用户的借阅历史
    [[nodiscard]] int getBorrowedBookCount() const; //获取当前已被借阅的图书数量
    [[nodiscard]] int getOverDueBookCount() const; //获取当前超期的图书数量

    [[nodiscard]] int getCurrentBorrowCount(const std::string &userId) const; //获取用户当前借阅的图书数量
    [[nodiscard]] int getTotalBorrowCount(const std::string &userId) const; //获取用户总借阅的图书数量
    [[nodiscard]] int getDueSoonCount(const std::string &userId) const; //获取用户即将到期的图书数量
    [[nodiscard]] int getOverdueCount(const std::string &userId) const; //获取用户超期的图书数量

    // 扩展
    [[nodiscard]] bool renewBook(const std::string &userId, const std::string &copyId); //续借

    static void setMaxBorrowCount(int maxBorrowCount); //设置最大借阅数量
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BORROWSERVICE_H
