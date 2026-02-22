#include "services/SearchService.h"

bool SearchService::searchBookById(const std::string &bookId, Book &book) const {
    return db->searchBookById(bookId, book);
}

std::vector<Book> SearchService::searchBooksByCategory(const std::string &category) const {
    return db->searchBooksByCategory(category);
}

std::vector<Book> SearchService::searchBookByAuthor(const std::string &author) const {
    return db->searchBookByAuthor(author);
}

bool SearchService::searchBookByTitle(const std::string &bookTitle, Book &book) const {
    return db->searchBookByTitle(bookTitle, book);
}
