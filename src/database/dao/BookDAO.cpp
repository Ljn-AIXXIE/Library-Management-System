#include "database/dao/BookDAO.h"

#include <iostream>

#include "common/Logger.h"

//添加图书
bool BookDAO::addBook(const Book &book, std::string &errorMessage) const {
    const std::string sql =
            "INSERT INTO book (id, title, author, category, publisher, publish_date, price, pages, description, copy_count) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    int copy_count = 0;

    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        errorMessage = bookDatabase->getLastError();
        Logger::getInstance().logError("BookDAO::addBook准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, book.getId().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, book.getCategory().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, book.getPublisher().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, book.getPublishDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, book.getPrice().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, book.getPages().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, book.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 10, copy_count);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        errorMessage = sqlite3_errmsg(bookDatabase->getDB());
        bookDatabase->setLastError(errorMessage);
        Logger::getInstance().logError("BookDAO::addBook执行SQL失败:" + bookDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//删除图书
bool BookDAO::deleteBook(const std::string &bookId) const {
    const std::string sql = "DELETE FROM book WHERE id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::deleteBook准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        bookDatabase->setLastError(sqlite3_errmsg(bookDatabase->getDB()));
        Logger::getInstance().logError("BookDAO::deleteBook执行SQL失败:" + bookDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//更新图书
bool BookDAO::updateBook(const Book &book) const {
    const std::string sql =
            "UPDATE book SET title=?, author=?, category=?, publisher=? ,publish_date=?, price=?, pages=?, description=? "
            "WHERE id=?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::updateBook准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getCategory().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, book.getPublisher().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, book.getPublishDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, book.getPrice().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, book.getPages().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, book.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, book.getId().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        bookDatabase->setLastError(sqlite3_errmsg(bookDatabase->getDB()));
        Logger::getInstance().logError("BookDAO::updateBook执行SQL失败:" + bookDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//用于获取列文本
static std::string columnText(sqlite3_stmt *stmt, int col) {
    const unsigned char *text = sqlite3_column_text(stmt, col);
    return text ? reinterpret_cast<const char *>(text) : "";
}

//从查询结果中提取图书信息
static Book extractBook(sqlite3_stmt *stmt) {
    Book book;
    book.setBookId(columnText(stmt, 0));
    book.setBookTitle(columnText(stmt, 1));
    book.setAuthor(columnText(stmt, 2));
    book.setCategory(columnText(stmt, 3));
    book.setPublisher(columnText(stmt, 4));
    book.setPublishDate(columnText(stmt, 5));
    book.setPrice(columnText(stmt, 6));
    book.setPages(columnText(stmt, 7));
    book.setDescription(columnText(stmt, 8));
    return book;
}

//根据图书id查询图书信息
bool BookDAO::searchBookById(const std::string &bookId, Book &book) const {
    const std::string sql =
            "SELECT id, title, author, category, publisher, publish_date, price, pages, description FROM book WHERE id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::searchBookById准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        book = extractBook(stmt);
        sqlite3_finalize(stmt);
        return true;
    }
    Logger::getInstance().logError("BookDAO::searchBookById执行SQL失败:" + bookDatabase->getLastError());
    sqlite3_finalize(stmt);
    return false;
}

//根据图书名称查询图书信息
bool BookDAO::searchBookByTitle(const std::string &bookTitle, Book &book) const {
    const std::string sql =
            "SELECT id, title, author, category, publisher, publish_date, price, pages, description FROM book WHERE title LIKE ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::searchBookByTitle准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    std::string pattern = "%" + bookTitle + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        book = extractBook(stmt);
        sqlite3_finalize(stmt);
        return true;
    }
    Logger::getInstance().logError("BookDAO::searchBookByTitle执行SQL失败:" + bookDatabase->getLastError());
    sqlite3_finalize(stmt);
    return false;
}

//获取所有图书
std::vector<Book> BookDAO::getAllBooks() const {
    std::vector<Book> books;
    const std::string sql =
            "SELECT id, title, author, category, publisher, publish_date, price, pages, description FROM book;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::getAllBooks准备SQL失败:" + bookDatabase->getLastError());
        return books;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        books.push_back(extractBook(stmt));
    }

    sqlite3_finalize(stmt);
    return books;
}

//根据图书分类查询图书信息
std::vector<Book> BookDAO::searchBooksByCategory(const std::string &category) const {
    std::vector<Book> books;
    const std::string sql =
            "SELECT id, title, author, category, publisher, publish_date, price, pages, description FROM book WHERE category LIKE ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::searchBooksByCategory准备SQL失败:" + bookDatabase->getLastError());
        return books;
    }

    std::string pattern = "%" + category + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        books.push_back(extractBook(stmt));
    }

    sqlite3_finalize(stmt);
    return books;
}

std::vector<Book> BookDAO::searchBookByAuthor(const std::string &author) const {
    std::vector<Book> books;
    const std::string sql =
            "SELECT id, title, author, category, publisher, publish_date, price, pages, description FROM book WHERE author LIKE ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::searchBookByAuthor准备SQL失败:" + bookDatabase->getLastError());
        return books;
    }

    std::string pattern = "%" + author + "%";

    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        books.push_back(extractBook(stmt));
    }

    sqlite3_finalize(stmt);
    return books;
}

std::string BookDAO::getBookTitleById(const std::string &bookId) const {
    const std::string sql = "SELECT title FROM book WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::getBookTitleById准备SQL失败:" + bookDatabase->getLastError());
        return "";
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    std::string title;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        title = columnText(stmt, 0);
    } else {
        Logger::getInstance().logError("BookDAO::getBookTitleById执行SQL失败:" + bookDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return title;
}

bool BookDAO::isBookIdExist(const std::string &bookId) const {
    const std::string sql = "SELECT COUNT(*) FROM book WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::isBookIdExist准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookDAO::isBookIdExist执行SQL失败:" + bookDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count > 0;
}

int BookDAO::getBookCopyCount(const std::string &bookId) const {
    const std::string sql = "SELECT copy_count FROM book WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::getBookCopyCount准备SQL失败:" + bookDatabase->getLastError());
        return 0;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookDAO::getBookCopyCount执行SQL失败:" + bookDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count;
}

bool BookDAO::updateBookCopyCount(const std::string &bookId) const {
    const std::string sql = "UPDATE book SET copy_count = copy_count + 1 WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookDAO::updateBookCopyCount准备SQL失败:" + bookDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookDAO::updateBookCopyCount执行SQL失败:" + bookDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
