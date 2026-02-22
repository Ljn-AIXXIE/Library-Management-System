#ifndef LIBRARY_MANAGEMENT_SYSTEM_RECORDDAO_H
#define LIBRARY_MANAGEMENT_SYSTEM_RECORDDAO_H

#include <vector>

#include "database/DatabaseOperator.h"
#include "entities/Record.h"

/*
CREATE TABLE record (
    user_id       TEXT NOT NULL,
    book_id       TEXT NOT NULL,
    copy_id       TEXT NOT NULL,
    borrow_time   INTEGER NOT NULL,
    return_time   INTEGER NOT NULL
);
*/

//用于封装对借阅信息的操作
class RecordDAO {
    DatabaseOperator *recordDatabase;

public:
    explicit RecordDAO(DatabaseOperator *recordDatabase) : recordDatabase(recordDatabase) {}

    //核心操作
    [[nodiscard]] bool addBorrowRecord(const Record &record) const; //为TxtToDatabaseWriter提供接口
    [[nodiscard]] bool addBorrowRecord(const std::string &userId, const std::string &copyId) const; //添加借阅记录
    [[nodiscard]] bool updateReturnTime(const std::string &userId, const std::string &copyId) const; //更新归还时间

    //查询操作
    [[nodiscard]] std::vector<Record> getActiveRecordsByUser(const std::string &userId) const; //获取用户当前借阅的记录
    [[nodiscard]] std::vector<Record> getHistoryRecordsByUser(const std::string &userId) const; //获取用户的借阅历史
    [[nodiscard]] std::vector<Record> getRecordsByCopyId(const std::string &copyId) const; //获取图书的借阅历史
    [[nodiscard]] bool getRecordCountByBookId(const std::string &bookId, int &count) const; //获取图书的借阅次数
    [[nodiscard]] bool getRecordCountByUserId(const std::string &userId, int &count) const; //获取用户的借阅次数
    [[nodiscard]] std::vector<Record> getActiveRecords() const; //获取当前借阅的记录数量

    //业务校验
    [[nodiscard]] bool hasActiveRecordByUserId(const std::string &userId, const std::string &copyId) const; //用于判断用户是否借阅该图书
    [[nodiscard]] bool hasActiveRecordByCopyId(const std::string &copyId) const; //用于判断图书是否已借出
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_RECORDDAO_H
