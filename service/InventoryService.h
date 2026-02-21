#ifndef AVL_BOOKSYSTEM_INVENTORYSERVICE_H
#define AVL_BOOKSYSTEM_INVENTORYSERVICE_H
#include "database/dao/BookDAO.h"
#include "database/dao/BookCopyDAO.h"

//用于管理库存，包括图书的添加、删除、修改和获取图书信息等操作
class InventoryService {
    BookDAO *db;
    BookCopyDAO *bookCopyDAO;

public:
    InventoryService(BookDAO *bookDAO, BookCopyDAO *bookCopyDAO);

    ~InventoryService();

    //核心操作
    [[nodiscard]] bool addBook(const Book &book) const; //添加图书
    [[nodiscard]] bool deleteBook(const string &bookId) const; //删除图书
    [[nodiscard]] bool updateBook(const Book &book) const; //更新图书
    [[nodiscard]] vector<Book> getAllBooks() const;

    [[nodiscard]] bool addBookCopy(const BookCopy &bookCopy) const; //添加图书副本
    [[nodiscard]] bool deleteBookCopy(const string &copyId) const; //删除指定图书副本
    [[nodiscard]] bool deleteAllBookCopy(const string &bookId) const; //删除指定图书的所有副本
    [[nodiscard]] vector<BookCopy> getBookCopies(const string &bookId) const; //获取所有图书副本
    [[nodiscard]] vector<BookCopy> getAvailableCopies(const string &bookId) const; //获取可借阅的副本
    [[nodiscard]] int getBookCopyCountByBookId(const string &bookId) const;

    [[nodiscard]] int getTotalCopyCount() const;

    [[nodiscard]] string getBookTitleById(const string &bookId) const; //根据图书id获取图书名称

    [[nodiscard]] int getAvailableCopyCount(const string &bookId) const;

    //业务校验
    [[nodiscard]] bool isBookIdExist(const string &bookId) const; //用于判断图书id是否已存在

    //辅助函数：用于生成副本id
    [[nodiscard]] string generateCopyId(const string &bookId) const;

    [[nodiscard]] bool updateBookCopyCount(const string &bookId) const;
};


#endif //AVL_BOOKSYSTEM_INVENTORYSERVICE_H
