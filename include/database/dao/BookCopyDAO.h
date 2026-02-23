#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKCOPYDAO_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKCOPYDAO_H

#include <string>

#include "database/DatabaseOperator.h"
#include "entities/BookCopy.h"

class BookCopyDAO {
    DatabaseOperator *bookCopyDatabase;

public:
    explicit BookCopyDAO(DatabaseOperator *bookDatabase) : bookCopyDatabase(bookDatabase) {
    }

    //核心操作
    [[nodiscard]] bool addBookCopy(const BookCopy &bookCopy) const; //添加图书副本
    [[nodiscard]] bool deleteBookCopy(const std::string &copyId) const; //删除图书副本
    [[nodiscard]] bool deleteAllBookCopy(const std::string &bookId) const; //删除指定图书的所有图书副本
    [[nodiscard]] bool updateBookCopyStatus(const std::string &copyId, const std::string &status) const; //更新图书副本状态

    //查询操作
    [[nodiscard]] std::vector<BookCopy> getCopiesByBookId(const std::string &bookId) const; //根据图书id获取所有副本
    [[nodiscard]] std::vector<BookCopy> getAvailableCopies(const std::string &bookId) const; //根据图书id获取所有可借阅的副本
    [[nodiscard]] int getCopyCountByBookId(const std::string &bookId) const; //根据图书id获取副本数量
    [[nodiscard]] int getAvailableCopyCount(const std::string &bookId) const; //根据图书id获取可借阅的副本数量
    [[nodiscard]] int getTotalCopyCount() const; //获取所有副本数量
    [[nodiscard]] int getBorrowedCopyCount() const; //获取已借出的副本数量

    //校验
    [[nodiscard]] bool isCopyBorrowed(const std::string &copyId) const; //根据副本id判断图书是否已借出
    [[nodiscard]] bool isCopyBookIdExist(const std::string &copyId) const; //根据副本id判断图书副本是否已存在
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKCOPYDAO_H
