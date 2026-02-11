#ifndef LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
#define LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
#include "../service/SearchService.h"
#include "../service/InventoryService.h"
#include "../service/BorrowService.h"
#include "../service/BlackListService.h"
#include "../utils/HttpUtils.h"


class BookController {
    SearchService *searchService;
    InventoryService *inventoryService;
    BorrowService *borrowService;
    BlackListService *blackListService;

public:
    explicit BookController(SearchService *searchService, InventoryService *inventoryService,
                            BorrowService *borrowService, BlackListService *blackListService);

    ~BookController();

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
};


#endif //LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
