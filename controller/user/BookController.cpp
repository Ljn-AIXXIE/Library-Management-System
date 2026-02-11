#include "BookController.h"

BookController::BookController(SearchService *searchService, InventoryService *inventoryService,
                               BorrowService *borrowService,
                               BlackListService *blackListService) : searchService(searchService),
                                                                     inventoryService(inventoryService),
                                                                     borrowService(borrowService),
                                                                     blackListService(blackListService) {
}

BookController::~BookController() = default;

void BookController::handleSearchBooks(const httplib::Request &req, httplib::Response &res) const {
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
    for (const auto &book: books) {
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

//GET /api/books/detail/book?isbn=<isbn> - 获取图书详细信息
void BookController::handleGetBookDetail(const httplib::Request &req, httplib::Response &res) const {
    cout << "[BookController] 获取图书详细信息" << endl;
    if (!req.has_param("isbn")) {
        cout << "获取图书详细信息失败: 缺少isbn参数" << endl;
        res = HttpUtils::createErrorResponse("缺少isbn参数", 400);
        return;
    }
    string bookId = req.get_param_value("isbn");
    Book book;
    if (!searchService->searchBookById(bookId, book)) {
        cout << "获取图书详细信息失败: 图书不存在" << endl;
        res = HttpUtils::createErrorResponse("图书不存在", 404);
        return;
    }

    int totalCopies = inventoryService->getBookCopyCount(bookId);
    int availableCopies = inventoryService->getAvailableCopyCount(bookId);
    int borrowedCopies = totalCopies - availableCopies;

    json responseData = {
        {"success", true},
        {
            "data", {
                {"isbn", book.getId()},
                {"title", book.getTitle()},
                {"author", book.getAuthor()},
                {"category", book.getCategory()},
                {"publisher", book.getPublisher()},
                {"publishDate", book.getPublishDate()},
                {"price", book.getPrice()},
                {"pages", book.getPages()},
                {"description", book.getDescription()},
                {"totalCount", totalCopies},
                {"availableCount", availableCopies}
            }
        }
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[BookController] 返回图书详细信息" << endl;
}

//GET /api/books/detail/copies?isbn=<isbn> - 获取图书副本信息
void BookController::handleGetBookCopies(const httplib::Request &req, httplib::Response &res) const {
    cout << "[BookController] 获取图书副本信息" << endl;
    if (!req.has_param("isbn")) {
        cout << "获取图书副本信息失败: 缺少isbn参数" << endl;
        res = HttpUtils::createErrorResponse("缺少isbn参数", 400);
        return;
    }
    string bookId = req.get_param_value("isbn");
    vector<BookCopy> copies = inventoryService->getBookCopies(bookId);
    json copiesArray = json::array();
    for (const auto &copy: copies) {
        copiesArray.push_back({
            {"copyId", copy.getCopyId()},
            {"status", copy.getStatus()}
        });
    }
    json responseData = {
        {"success", true},
        {"data", copiesArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[BookController] 返回 " << copies.size() << " 个副本" << endl;
}

//POST /api/borrow - 借阅图书
void BookController::handleBorrowBook(const httplib::Request &req, httplib::Response &res) const {
    cout << "[BookController] 借阅图书" << endl;
    json requestData = HttpUtils::parseRequestBody(req);
    string userId = requestData["userId"];
    string bookId = requestData["isbn"];
    string copyId = requestData["copyId"];

    //验证用户是不是在黑名单中
    if (blackListService->isBlackListed(userId)) {
        cout << "借阅失败: 用户已被拉黑" << endl;
        res = HttpUtils::createErrorResponse("用户已被拉黑", 403);
        return;
    }

    //验证用户是否可以继续借阅
    if (!borrowService->canUserBorrowMore(userId)) {
        cout << "借阅失败: 用户已达到借阅上限" << endl;
        res = HttpUtils::createErrorResponse("用户已达到借阅上限", 403);
        return;
    }

    if (borrowService->borrowBook(userId, copyId)) {
        json responseData = {
            {"success", true},
            {"message", "借阅成功,请按时归还"}
        };
        cout << "借阅成功" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        cout << "借阅失败" << endl;
        res = HttpUtils::createErrorResponse("借阅失败", 500);
    }
}

//辅助函数：返回时间的字符串表示
string formatTime(time_t timestamp) {
    char buffer[20];

    tm *timeInfo = std::localtime(&timestamp);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeInfo);

    return string(buffer);
}

//GET /api/borrow/current - 获取用户当前借阅的记录
void BookController::handleGetUserBorrowingRecords(const httplib::Request &req, httplib::Response &res) const {
    cout << "[BookController] 获取用户当前借阅的记录" << endl;

    string userId = req.get_param_value("userId");
    vector<Record> records = borrowService->getUserBorrowingRecords(userId);
    json recordsArray = json::array();
    for (const auto &record: records) {
        string bookId = record.getCopyId().substr(0, record.getCopyId().find_last_of('_'));
        string bookTitle = inventoryService->getBookTitleById(bookId);
        string dueTime = formatTime(record.getDueTime());
        string borrowTime = formatTime(record.getBorrowTime());

        recordsArray.push_back({
            {"dueDate", dueTime},
            {"bookTitle", bookTitle},
            {"copyId", record.getCopyId()},
            {"borrowDate", borrowTime}
        });
    }
    json responseData = {
        {"success", true},
        {"data", recordsArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[BookController] 返回 " << records.size() << " 条记录" << endl;
}

//GET /api/borrow/history - 获取用户借阅历史
void BookController::handleGetUserBorrowHistory(const httplib::Request &req, httplib::Response &res) const {
    cout << "[BookController] 获取用户借阅历史" << endl;

    string userId = req.get_param_value("userId");
    vector<Record> records = borrowService->getUserHistoryRecords(userId);
    json recordsArray = json::array();

    for (const auto &record: records) {
        string bookId = record.getCopyId().substr(0, record.getCopyId().find_last_of('_'));
        string bookTitle = inventoryService->getBookTitleById(bookId);
        string returnTime = formatTime(record.getReturnTime());
        string borrowTime = formatTime(record.getBorrowTime());

        recordsArray.push_back({
            {"returnDate", returnTime},
            {"bookTitle", bookTitle},
            {"borrowDate", borrowTime}
        });
    }
    json responseData = {
        {"success", true},
        {"data", recordsArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[BookController] 返回 " << records.size() << " 条记录" << endl;
}

//POST /api/borrow/return - 归还图书
void BookController::handleReturnBook(const httplib::Request &req, httplib::Response &res) const {
    cout << "[BookController] 归还图书" << endl;
    json requestData = HttpUtils::parseRequestBody(req);
    string userId = requestData["userId"];
    string copyId = requestData["copyId"];
    if (borrowService->returnBook(userId, copyId)) {
        json responseData = {
            {"success", true},
            {"message", "归还成功"}
        };
        cout << "归还成功" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        cout << "归还失败" << endl;
        res = HttpUtils::createErrorResponse("归还失败", 500);
    }
}
