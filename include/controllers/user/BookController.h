#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKSEARCHCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKSEARCHCONTROLLER_H

#include "services/BlackListService.h"
#include "services/BorrowService.h"
#include "services/InventoryService.h"
#include "services/SearchService.h"
#include "utils/HttpUtils.h"

class BookController {
    SearchService *searchService;
    InventoryService *inventoryService;
    BorrowService *borrowService;
    BlackListService *blackListService;
public:
    explicit BookController(SearchService *searchService, InventoryService *inventoryService,
                            BorrowService *borrowService, BlackListService *blackListService)
        : searchService(searchService), inventoryService(inventoryService), borrowService(borrowService), blackListService(blackListService) {}

    //GET /api/books/search - 搜索图书
    void handleSearchBooks(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/books/detail/book?isbn=<isbn> - 获取图书详细信息
    void handleGetBookDetail(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/books/detail/copies?isbn=<isbn> - 获取图书副本信息
    void handleGetBookCopies(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/borrow - 借阅图书
    void handleBorrowBook(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/borrow/current - 获取用户当前借阅的记录
    void handleGetUserBorrowingRecords(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/borrow/history - 获取用户借阅历史
    void handleGetUserBorrowHistory(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/borrow/return - 归还图书
    void handleReturnBook(const httplib::Request &req, httplib::Response &res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKSEARCHCONTROLLER_H
