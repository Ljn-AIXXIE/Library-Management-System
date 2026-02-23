#include "services/InventoryService.h"

#include "common/Logger.h"

bool InventoryService::addBook(const Book &book) const {
    Logger::getInstance().logBusiness("添加图书" + book.getId());
    if (db->addBook(book)) {
        Logger::getInstance().logBusiness("添加图书" + book.getId() + "成功");
        return true;
    }
    Logger::getInstance().logError("添加图书" + book.getId() + "失败");
    return false;
}

bool InventoryService::deleteBook(const std::string &bookId) const {
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

std::vector<Book> InventoryService::getAllBooks() const {
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

bool InventoryService::deleteBookCopy(const std::string &copyId) const {
    Logger::getInstance().logBusiness("删除图书副本" + copyId);
    if (bookCopyDAO->deleteBookCopy(copyId)) {
        Logger::getInstance().logBusiness("删除图书副本" + copyId + "成功");
        return true;
    }
    Logger::getInstance().logError("删除图书副本" + copyId + "失败");
    return false;
}

bool InventoryService::deleteAllBookCopy(const std::string &bookId) const {
    Logger::getInstance().logBusiness("删除所有图书副本" + bookId);
    if (bookCopyDAO->deleteAllBookCopy(bookId)) {
        Logger::getInstance().logBusiness("删除所有图书副本" + bookId + "成功");
        return true;
    }
    Logger::getInstance().logError("删除所有图书副本" + bookId + "失败");
    return false;
}

std::vector<BookCopy> InventoryService::getBookCopies(const std::string &bookId) const {
    return bookCopyDAO->getCopiesByBookId(bookId);
}

std::vector<BookCopy> InventoryService::getAvailableCopies(const std::string &bookId) const {
    return bookCopyDAO->getAvailableCopies(bookId);
}

int InventoryService::getBookCopyCountByBookId(const std::string &bookId) const {
    return bookCopyDAO->getCopyCountByBookId(bookId);
}

int InventoryService::getTotalCopyCount() const {
    return bookCopyDAO->getTotalCopyCount();
}

std::string InventoryService::getBookTitleById(const std::string &bookId) const {
    return db->getBookTitleById(bookId);
}

int InventoryService::getAvailableCopyCount(const std::string &bookId) const {
    return bookCopyDAO->getAvailableCopyCount(bookId);
}

bool InventoryService::isBookIdExist(const std::string &bookId) const {
    return db->isBookIdExist(bookId);
}

bool InventoryService::isCopyBookIdExist(const std::string &copyId) const {
    return bookCopyDAO->isCopyBookIdExist(copyId);
}

std::string InventoryService::generateCopyId(const std::string &bookId) const {
    int count = db->getBookCopyCount(bookId);
    return bookId + "_" + std::to_string(count + 1);
}

bool InventoryService::updateBookCopyCount(const std::string &bookId) const {
    return db->updateBookCopyCount(bookId);
}
