#include "controllers/user/BookController.h"

#include "utils/TimeUtils.h"
#include "common/Logger.h"

using std::string;
using std::vector;

void BookController::handleSearchBooks(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/books/search 搜索图书");

    if (!req.has_param("type") || !req.has_param("keyword")) {
        res = HttpUtils::createErrorResponse("缺少type或keyword参数", 400);
        return;
    }
    string type = req.get_param_value("type");
    string keyword = req.get_param_value("keyword");
    vector<Book> books;

    if (type == "title") {
        Book book;
        if (!searchService->searchBookByTitle(keyword, book)) {
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
        books.push_back(book);
    } else if (type == "author") {
        books = searchService->searchBookByAuthor(keyword);
        if (books.empty()) {
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
    } else if (type == "category") {
        books = searchService->searchBooksByCategory(keyword);
        if (books.empty()) {
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
    } else if (type == "isbn") {
        Book book;
        if (!searchService->searchBookById(keyword, book)) {
            res = HttpUtils::createErrorResponse("图书不存在", 404);
            return;
        }
        books.push_back(book);
    } else {
        res = HttpUtils::createErrorResponse("未知的搜索类型", 400);
        return;
    }

    nlohmann::json booksArray = nlohmann::json::array();
    for (const auto &book: books) {
        booksArray.push_back({
            {"isbn", book.getId()},
            {"title", book.getTitle()},
            {"author", book.getAuthor()},
            {"category", book.getCategory()},
            {"totalCount", inventoryService->getBookCopyCountByBookId(book.getId())},
            {"availableCount", inventoryService->getAvailableCopyCount(book.getId())}
        });
    }
    nlohmann::json responseData = {
        {"success", true},
        {"data", booksArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

//GET /api/books/detail/book?isbn=<isbn> - 获取图书详细信息
void BookController::handleGetBookDetail(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/books/detail/book 获取图书详细信息");

    if (!req.has_param("isbn")) {
        res = HttpUtils::createErrorResponse("缺少isbn参数", 400);
        return;
    }
    string bookId = req.get_param_value("isbn");
    Book book;
    if (!searchService->searchBookById(bookId, book)) {
        res = HttpUtils::createErrorResponse("图书不存在", 404);
        return;
    }

    int totalCopies = inventoryService->getBookCopyCountByBookId(bookId);
    int availableCopies = inventoryService->getAvailableCopyCount(bookId);

    nlohmann::json responseData = {
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
}

//GET /api/books/detail/copies?isbn=<isbn> - 获取图书副本信息
void BookController::handleGetBookCopies(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/books/detail/copies 获取图书副本信息");
    if (!req.has_param("isbn")) {
        res = HttpUtils::createErrorResponse("缺少isbn参数", 400);
        return;
    }
    string bookId = req.get_param_value("isbn");
    vector<BookCopy> copies = inventoryService->getBookCopies(bookId);
    nlohmann::json copiesArray = nlohmann::json::array();
    for (const auto &copy: copies) {
        copiesArray.push_back({
            {"copyId", copy.getCopyId()},
            {"status", copy.getStatus()}
        });
    }
    nlohmann::json responseData = {
        {"success", true},
        {"data", copiesArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

//POST /api/borrow - 借阅图书
void BookController::handleBorrowBook(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/borrow 借阅图书");

    nlohmann::json requestData = HttpUtils::parseRequestBody(req);
    string userId = requestData["userId"];
    string bookId = requestData["isbn"];
    string copyId = requestData["copyId"];

    //验证用户是不是在黑名单中
    if (blackListService->isBlackListed(userId)) {
        res = HttpUtils::createErrorResponse("用户已被拉黑", 403);
        return;
    }

    //验证用户是否可以继续借阅
    if (!borrowService->canUserBorrowMore(userId)) {
        res = HttpUtils::createErrorResponse("用户已达到借阅上限", 403);
        return;
    }

    if (borrowService->borrowBook(userId, copyId)) {
        nlohmann::json responseData = {
            {"success", true},
            {"message", "借阅成功,请按时归还"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        res = HttpUtils::createErrorResponse("借阅失败", 500);
    }
}

//GET /api/borrow/current - 获取用户当前借阅的记录
void BookController::handleGetUserBorrowingRecords(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/borrow/current 获取用户当前借阅的记录");

    string userId = req.get_param_value("userId");
    vector<Record> records = borrowService->getUserBorrowingRecords(userId);
    nlohmann::json recordsArray = nlohmann::json::array();
    for (const auto &record: records) {
        string bookId = record.getCopyId().substr(0, record.getCopyId().find_last_of('_'));
        string bookTitle = inventoryService->getBookTitleById(bookId);
        string dueTime = TimeUtils::formatTime(record.getDueTime());
        string borrowTime = TimeUtils::formatTime(record.getBorrowTime());

        recordsArray.push_back({
            {"dueDate", dueTime},
            {"bookTitle", bookTitle},
            {"copyId", record.getCopyId()},
            {"borrowDate", borrowTime}
        });
    }
    nlohmann::json responseData = {
        {"success", true},
        {"data", recordsArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

//GET /api/borrow/history - 获取用户借阅历史
void BookController::handleGetUserBorrowHistory(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/borrow/history 获取用户借阅历史");

    string userId = req.get_param_value("userId");
    vector<Record> records = borrowService->getUserHistoryRecords(userId);
    nlohmann::json recordsArray = nlohmann::json::array();

    for (const auto &record: records) {
        string bookId = record.getCopyId().substr(0, record.getCopyId().find_last_of('_'));
        string bookTitle = inventoryService->getBookTitleById(bookId);
        string returnTime = TimeUtils::formatTime(record.getReturnTime());
        string borrowTime = TimeUtils::formatTime(record.getBorrowTime());

        recordsArray.push_back({
            {"returnDate", returnTime},
            {"bookTitle", bookTitle},
            {"borrowDate", borrowTime}
        });
    }
    nlohmann::json responseData = {
        {"success", true},
        {"data", recordsArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

//POST /api/borrow/return - 归还图书
void BookController::handleReturnBook(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/borrow/return 归还图书");

    nlohmann::json requestData = HttpUtils::parseRequestBody(req);
    string userId = requestData["userId"];
    string copyId = requestData["copyId"];
    if (borrowService->returnBook(userId, copyId)) {
        nlohmann::json responseData = {
            {"success", true},
            {"message", "归还成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        res = HttpUtils::createErrorResponse("归还失败", 500);
    }
}
