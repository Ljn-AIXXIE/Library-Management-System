#ifndef LIBRARY_MANAGEMENT_SYSTEM_SEARCHSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_SEARCHSERVICE_H

#include "database/dao/BookDAO.h"

//用于管理图书的查询等操作
class SearchService {
    BookDAO *db;
public:
    explicit SearchService(BookDAO *bookDAO) : db(bookDAO) {}

    [[nodiscard]] bool searchBookById(const std::string &bookId, Book &book) const; //根据图书id查询图书信息
    [[nodiscard]] std::vector<Book> searchBooksByCategory(const std::string &category) const; //根据图书分类查询图书信息
    [[nodiscard]] std::vector<Book> searchBookByAuthor(const std::string &author) const; //根据图书作者查询图书信息
    [[nodiscard]] bool searchBookByTitle(const std::string &bookTitle, Book &book) const; //根据图书名称查询图书信息
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_SEARCHSERVICE_H
