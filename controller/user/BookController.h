#ifndef LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
#define LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
#include "../service/SearchService.h"
#include "../service/InventoryService.h"
#include "../utils/HttpUtils.h"


class BookController {
    SearchService *searchService;
    InventoryService *inventoryService;
public:
    explicit BookController(SearchService *searchService, InventoryService *inventoryService);
    ~BookController();

    //GET /api/books/search - 搜索图书
    void handleSearchBooks(const httplib::Request& req, httplib::Response& res) const;

    //GET /api/books/detail/book?isbn=<isbn> - 获取图书详细信息
    void handleGetBookDetail(const httplib::Request& req, httplib::Response& res) const;

    //GET /api/books/detail/copies?isbn=<isbn> - 获取图书副本信息
    void handleGetBookCopies(const httplib::Request& req, httplib::Response& res) const;
};


#endif //LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H