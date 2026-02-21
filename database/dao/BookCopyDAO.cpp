#include "BookCopyDAO.h"
#include "common/Logger.h"

/*
CREATE TABLE book_copy (
copy_id     TEXT PRIMARY KEY,
book_id     TEXT NOT NULL,
status      TEXT NOT NULL,   -- available / borrowed
);
*/

BookCopyDAO::BookCopyDAO(DatabaseOperator *bookDatabase) : bookCopyDatabase(bookDatabase) {
}

BookCopyDAO::~BookCopyDAO() = default;

//添加图书副本
bool BookCopyDAO::addBookCopy(const BookCopy &bookCopy) const {
    const string sql =
            "INSERT INTO book_copy (copy_id, book_id, status) "
            "VALUES (?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::addBookCopy准备SQL失败:" + bookCopyDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookCopy.getCopyId().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, bookCopy.getBookId().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, bookCopy.getStatus().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookCopyDAO::addBookCopy执行SQL失败:" + bookCopyDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//删除图书副本
bool BookCopyDAO::deleteBookCopy(const string &copyId) const {
    const string sql = "DELETE FROM book_copy WHERE copy_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::deleteBookCopy准备SQL失败:" + bookCopyDatabase->getLastError());
        return false;
    }
    sqlite3_bind_text(stmt, 1, copyId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookCopyDAO::deleteBookCopy执行SQL失败:" + bookCopyDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//删除指定图书的所有图书副本
bool BookCopyDAO::deleteAllBookCopy(const string &bookId) const {
    const string sql = "DELETE FROM book_copy WHERE book_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::deleteAllBookCopy准备SQL失败:" + bookCopyDatabase->getLastError());
        return false;
    }
    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookCopyDAO::deleteAllBookCopy执行SQL失败:" + bookCopyDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//更新图书副本状态
bool BookCopyDAO::updateBookCopyStatus(const string &copyId, const string &status) const {
    const string sql = "UPDATE book_copy SET status = ? WHERE copy_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::updateBookCopyStatus准备SQL失败:" + bookCopyDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, copyId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookCopyDAO::updateBookCopyStatus执行SQL失败:" + bookCopyDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

int BookCopyDAO::getCopyCountByBookId(const string &bookId) const {
    const string sql = "SELECT COUNT(*) FROM book_copy WHERE book_id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::getCopyCountByBookId准备SQL失败:" + bookCopyDatabase->getLastError());
        return 0;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookCopyDAO::getCopyCountByBookId执行SQL失败:" + bookCopyDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count;
}

int BookCopyDAO::getAvailableCopyCount(const string &bookId) const {
    const string sql = "SELECT COUNT(*) FROM book_copy WHERE book_id = ? AND status = 'available';";
    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::getAvailableCopyCount准备SQL失败:" + bookCopyDatabase->getLastError());
        return 0;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookCopyDAO::getAvailableCopyCount执行SQL失败:" + bookCopyDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count;
}

//获取所有副本数量
int BookCopyDAO::getTotalCopyCount() const {
    const std::string sql = "SELECT COUNT(*) FROM book_copy;";
    sqlite3_stmt *stmt = nullptr;

    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::getTotalCopyCount准备SQL失败:" + bookCopyDatabase->getLastError());
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookCopyDAO::getTotalCopyCount执行SQL失败:" + bookCopyDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count;
}

//获取已借出的副本数量
int BookCopyDAO::getBorrowedCopyCount() const {
    const std::string sql = "SELECT COUNT(*) FROM book_copy WHERE status = 'borrowed';";
    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::getBorrowedCopyCount准备SQL失败:" + bookCopyDatabase->getLastError());
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookCopyDAO::getBorrowedCopyCount执行SQL失败:" + bookCopyDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count;
}

//用于获取列文本
static string columnText(sqlite3_stmt *stmt, int col) {
    const unsigned char *text = sqlite3_column_text(stmt, col);
    return text ? reinterpret_cast<const char *>(text) : "";
}

//根据图书id获取所有副本
vector<BookCopy> BookCopyDAO::getCopiesByBookId(const string &bookId) const {
    vector<BookCopy> copies;
    const string sql =
            "SELECT copy_id, book_id, status "
            "FROM book_copy "
            "WHERE book_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::getCopiesByBookId准备SQL失败:" + bookCopyDatabase->getLastError());
        return copies;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        BookCopy copy;
        copy.setCopyId(columnText(stmt, 0));
        copy.setBookId(columnText(stmt, 1));
        copy.setStatus(columnText(stmt, 2));
        copies.push_back(copy);
    }

    sqlite3_finalize(stmt);
    return copies;
}

//根据图书id获取所有可借阅的副本
vector<BookCopy> BookCopyDAO::getAvailableCopies(const string &bookId) const {
    vector<BookCopy> copies;
    const string sql =
            "SELECT copy_id, book_id, status "
            "FROM book_copy "
            "WHERE book_id = ? AND status = 'available';";

    sqlite3_stmt *stmt = nullptr;
    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::getAvailableCopies准备SQL失败:" + bookCopyDatabase->getLastError());
        return copies;
    }

    sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        BookCopy copy;
        copy.setCopyId(columnText(stmt, 0));
        copy.setBookId(columnText(stmt, 1));
        copy.setStatus(columnText(stmt, 2));
        copies.push_back(copy);
    }

    sqlite3_finalize(stmt);
    return copies;
}

//根据副本id判断图书是否已借出
bool BookCopyDAO::isCopyBorrowed(const string &copyId) const {
    const string sql = "SELECT COUNT(*) FROM book_copy WHERE copy_id = ? AND status = 'borrowed';";
    sqlite3_stmt *stmt = nullptr;

    if (!bookCopyDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookCopyDAO::isCopyBorrowed准备SQL失败:" + bookCopyDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, copyId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BookCopyDAO::isCopyBorrowed执行SQL失败:" + bookCopyDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count > 0;
}
