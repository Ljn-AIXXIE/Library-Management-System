#ifndef AVL_BOOKSYSTEM_ADMINBOOKCONTROLLER_H
#define AVL_BOOKSYSTEM_ADMINBOOKCONTROLLER_H
#include "HttpUtils.h"
#include "../service/InventoryService.h"
#include "../service/SearchService.h"
#include "../database/dao/BookCopyDAO.h"

//图书管理（添加，编辑，删除图书及副本）
class AdminBookController {
    InventoryService *inventoryService;
    SearchService *searchService;
public:
    explicit AdminBookController(InventoryService *inventoryService, SearchService *searchService);
    ~AdminBookController();

    //GET /api/admin/books - 获取所有图书
    void handleGetAllBooks(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/admin/books/add - 添加图书
    void handleAddBook(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/admin/books/update - 更新图书
    void handleUpdateBook(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/admin/books/delete - 删除图书
    void handleDeleteBook(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/admin/copies?isbn=<isbn> - 获取所有图书副本
    void handleGetAllBookCopies(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/admin/copies/add - 添加图书副本
    void handleAddBookCopy(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/admin/copies/delete - 删除图书副本
    void handleDeleteBookCopy(const httplib::Request &req, httplib::Response &res) const;
};


#endif //AVL_BOOKSYSTEM_ADMINBOOKCONTROLLER_H