#ifndef LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
#define LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H
#include "../service/SearchService.h"
#include "../service/InventoryService.h"
#include "../utils/HttpUtils.h"


class BookSearchController {
    SearchService *searchService;
    InventoryService *inventoryService;
public:
    explicit BookSearchController(SearchService *searchService, InventoryService *inventoryService);
    ~BookSearchController();

    //GET /api/books/search - 搜索图书
    void handleSearchBooks(const httplib::Request& req, httplib::Response& res) const;
};


#endif //LIBRARYMANAGEMENTSYSTEM_BOOKSEARCHCONTROLLER_H