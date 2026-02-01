#include "InventoryService.h"

InventoryService::InventoryService(BookDAO* bookDAO, BookCopyDAO* bookCopyDAO)
: db(bookDAO), bookCopyDAO(bookCopyDAO) {}

InventoryService::~InventoryService() = default;

bool InventoryService::addBook(const Book& book) const {
    return db->addBook(book);
}

bool InventoryService::deleteBook(const string& bookId) const {
    return db->deleteBook(bookId);
}

bool InventoryService::addBookCopy(const BookCopy& bookCopy) const {
    return bookCopyDAO->addBookCopy(bookCopy);
}

bool InventoryService::deleteBookCopy(const string& copyId) const {
    return bookCopyDAO->deleteBookCopy(copyId);
}

bool InventoryService::updateBook(const Book& book) const {
    return db->updateBook(book);
}

vector<Book> InventoryService::getAllBooks() const {
    return db->getAllBooks();
}

vector<BookCopy> InventoryService::getBookCopies(const string& bookId) const {
    return bookCopyDAO->getCopiesByBookId(bookId);
}

vector<BookCopy> InventoryService::getAvailableCopies(const string& bookId) const {
    return bookCopyDAO->getAvailableCopies(bookId);
}

bool InventoryService::isBookIdExist(const string& bookId) const {
    return db->isBookIdExist(bookId);
}

string InventoryService::generateCopyId(const string& bookId) const {
    int count = db->getBookCopyCount(bookId);
    return bookId + "_" + to_string(count + 1);
}

bool InventoryService::updateBookCopyCount(const string& bookId) const {
    return db->updateBookCopyCount(bookId);
}