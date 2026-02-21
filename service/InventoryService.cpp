#include "InventoryService.h"
#include "common/Logger.h"

InventoryService::InventoryService(BookDAO *bookDAO, BookCopyDAO *bookCopyDAO)
    : db(bookDAO), bookCopyDAO(bookCopyDAO) {
}

InventoryService::~InventoryService() = default;

bool InventoryService::addBook(const Book &book) const {
    Logger::getInstance().logBusiness("添加图书" + book.getId());
    if (db->addBook(book)) {
        Logger::getInstance().logBusiness("添加图书" + book.getId() + "成功");
        return true;
    }
    Logger::getInstance().logError("添加图书" + book.getId() + "失败");
    return false;
}

bool InventoryService::deleteBook(const string &bookId) const {
    Logger::getInstance().logBusiness("删除图书" + bookId);
    if (db->deleteBook(bookId)) {
        Logger::getInstance().logBusiness("删除图书" + bookId + "成功");
        return true;
    }
    Logger::getInstance().logError("删除图书" + bookId + "失败");
    return false;
}

bool InventoryService::updateBook(const Book &book) const {
    Logger::getInstance().logBusiness("更新图书" + book.getId());
    if (db->updateBook(book)) {
        Logger::getInstance().logBusiness("更新图书" + book.getId() + "成功");
        return true;
    }
    Logger::getInstance().logError("更新图书" + book.getId() + "失败");
    return false;
}

vector<Book> InventoryService::getAllBooks() const {
    return db->getAllBooks();
}


bool InventoryService::addBookCopy(const BookCopy &bookCopy) const {
    Logger::getInstance().logBusiness("添加图书副本" + bookCopy.getCopyId());
    if (bookCopyDAO->addBookCopy(bookCopy)) {
        Logger::getInstance().logBusiness("添加图书副本" + bookCopy.getCopyId() + "成功");
        return true;
    }
    Logger::getInstance().logError("添加图书副本" + bookCopy.getCopyId() + "失败");
    return false;
}

bool InventoryService::deleteBookCopy(const string &copyId) const {
    Logger::getInstance().logBusiness("删除图书副本" + copyId);
    if (bookCopyDAO->deleteBookCopy(copyId)) {
        Logger::getInstance().logBusiness("删除图书副本" + copyId + "成功");
        return true;
    }
    Logger::getInstance().logError("删除图书副本" + copyId + "失败");
    return false;
}

bool InventoryService::deleteAllBookCopy(const string &bookId) const {
    Logger::getInstance().logBusiness("删除所有图书副本" + bookId);
    if (bookCopyDAO->deleteAllBookCopy(bookId)) {
        Logger::getInstance().logBusiness("删除所有图书副本" + bookId + "成功");
        return true;
    }
    Logger::getInstance().logError("删除所有图书副本" + bookId + "失败");
    return false;
}

vector<BookCopy> InventoryService::getBookCopies(const string &bookId) const {
    return bookCopyDAO->getCopiesByBookId(bookId);
}

vector<BookCopy> InventoryService::getAvailableCopies(const string &bookId) const {
    return bookCopyDAO->getAvailableCopies(bookId);
}

int InventoryService::getBookCopyCountByBookId(const string &bookId) const {
    return bookCopyDAO->getCopyCountByBookId(bookId);
}

int InventoryService::getTotalCopyCount() const {
    return bookCopyDAO->getTotalCopyCount();
}

string InventoryService::getBookTitleById(const string &bookId) const {
    return db->getBookTitleById(bookId);
}

int InventoryService::getAvailableCopyCount(const string &bookId) const {
    return bookCopyDAO->getAvailableCopyCount(bookId);
}

bool InventoryService::isBookIdExist(const string &bookId) const {
    return db->isBookIdExist(bookId);
}

string InventoryService::generateCopyId(const string &bookId) const {
    int count = db->getBookCopyCount(bookId);
    return bookId + "_" + std::to_string(count + 1);
}

bool InventoryService::updateBookCopyCount(const string &bookId) const {
    return db->updateBookCopyCount(bookId);
}
