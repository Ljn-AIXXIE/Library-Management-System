#include "BookSearchController.h"

BookSearchController::BookSearchController(SearchService *searchService,InventoryService *inventoryService) : searchService(searchService),inventoryService(inventoryService) {}

BookSearchController::~BookSearchController() = default;

void BookSearchController::handleSearchBooks(const httplib::Request& req, httplib::Response& res) const {
    cout << "[BookSearchController] 搜索图书" << endl;
    if (!req.has_param("type") || !req.has_param("keyword")) {
        cout << "搜索图书失败: 缺少type或keyword参数" << endl;
        res = HttpUtils::createErrorResponse("缺少type或keyword参数", 400);
        return;
    }
    string type = req.get_param_value("type");
    string keyword = req.get_param_value("keyword");
    vector<Book> books;
    if (type == "title") {
        Book book;
        if (!searchService->searchBookByTitle(keyword, book)) {
            cout << "搜索图书失败: 图书不存在" << endl;
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
        books.push_back(book);
    } else if (type == "author") {
        books = searchService->searchBookByAuthor(keyword);
        if (books.empty()) {
            cout << "搜索图书失败: 图书不存在" << endl;
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
    } else if (type == "category") {
        books = searchService->searchBooksByCategory(keyword);
        if (books.empty()) {
            cout << "搜索图书失败: 图书不存在" << endl;
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
    } else if (type == "isbn") {
        Book book;
        if (!searchService->searchBookById(keyword, book)) {
            cout << "搜索图书失败: 图书不存在" << endl;
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
        books.push_back(book);
    } else {
        cout << "搜索图书失败: 未知的搜索类型" << endl;
        res = HttpUtils::createErrorResponse("未知的搜索类型", 400);
        return;
    }

    json booksArray = json::array();
    for (const auto& book : books) {
        booksArray.push_back({
            {"isbn", book.getId()},
            {"title", book.getTitle()},
            {"author", book.getAuthor()},
            {"category", book.getCategory()},
            {"totalCount", inventoryService->getBookCopyCount(book.getId())},
            {"availableCount", inventoryService->getAvailableCopyCount(book.getId())}
        });
    }
    json responseData = {
        {"success", true},
        {"data", booksArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[BookSearchController] 返回 " << books.size() << " 本图书" << endl;
}