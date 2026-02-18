#include "AdminBookController.h"
#include "../common/Logger.h"

AdminBookController::AdminBookController(InventoryService *inventoryService,
                                         SearchService *searchService) : inventoryService(inventoryService),
                                                                         searchService(searchService) {
}

AdminBookController::~AdminBookController() = default;

//GET /api/admin/books - 获取所有图书
void AdminBookController::handleGetAllBooks(httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/books 获取所有图书");

    vector<Book> books = inventoryService->getAllBooks();

    json booksArray = json::array();
    for (const auto &book: books) {
        booksArray.push_back({
            {"isbn", book.getId()},
            {"title", book.getTitle()},
            {"author", book.getAuthor()},
            {"category", book.getCategory()},
            {"publisher", book.getPublisher()},
            {"totalCount", inventoryService->getBookCopyCount(book.getId())},
            {"availableCount", inventoryService->getAvailableCopyCount(book.getId())}
        });
    }

    json responseData = {
        {"success", true},
        {"data", booksArray}
    };

    res = HttpUtils::createSuccessResponse(responseData, 200);
}

//POST /api/admin/books/add - 添加图书
void AdminBookController::handleAddBook(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/admin/books/add 添加图书");

    json requestData = HttpUtils::parseRequestBody(req);

    // 验证必填字段
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"isbn", "title", "author", "category"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    // 构建图书对象
    string bookId = requestData["isbn"];

    //检查图书是否已存在,不存在需要在Book表中添加，存在则直接在BookCopy表中添加副本
    if (!inventoryService->isBookIdExist(bookId)) {
        string title = requestData["title"];
        string author = requestData["author"];
        string category = requestData["category"];
        string publisher = requestData.value("publisher", "");
        string publishDate = requestData.value("publishDate", "");
        string price = requestData.value("price", "");
        string pages = requestData.value("pages", "");
        string description = requestData.value("description", "");
        Book book = Book(bookId, title, author, category);

        if (!publisher.empty()) book.setPublisher(publisher);
        if (!publishDate.empty()) book.setPublishDate(publishDate);
        if (!price.empty()) book.setPrice(price);
        if (!pages.empty()) book.setPages(pages);
        if (!description.empty()) book.setDescription(description);

        if (inventoryService->addBook(book)) {
            json responseData = {
                {"success", true},
                {"message", "图书添加成功"}
            };
            res = HttpUtils::createSuccessResponse(responseData, 201);
        } else {
            res = HttpUtils::createErrorResponse("图书添加失败", 500);
        }
    }

    string copyId = inventoryService->generateCopyId(bookId);
    BookCopy bookCopy = BookCopy(bookId, copyId, "available");
    if (inventoryService->addBookCopy(bookCopy) && inventoryService->updateBookCopyCount(bookId)) {
        json responseData = {
            {"success", true},
            {"message", "图书副本添加成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        res = HttpUtils::createErrorResponse("图书副本添加失败", 500);
    }
}

//POST /api/admin/books - 更新图书
void AdminBookController::handleUpdateBook(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/admin/books 更新图书");

    json requestData = HttpUtils::parseRequestBody(req);

    // 获取旧图书对象
    string bookId = requestData["isbn"];
    Book oldBook;
    if (!searchService->searchBookById(bookId, oldBook)) {
        res = HttpUtils::createErrorResponse("图书不存在", 404);
        return;
    }

    auto getStringOrOld = [&](const json &reqData, const string &oldValue) {
        if (reqData.is_null()) return oldValue;
        if (reqData.is_string() && reqData.get<string>().empty()) return oldValue;
        return reqData.get<string>();
    };

    string title = getStringOrOld(requestData["title"], oldBook.getTitle());
    string author = getStringOrOld(requestData["author"], oldBook.getAuthor());
    string category = getStringOrOld(requestData["category"], oldBook.getCategory());
    string publisher = getStringOrOld(requestData["publisher"], oldBook.getPublisher());
    string publishDate = getStringOrOld(requestData["publishDate"], oldBook.getPublishDate());
    string price = getStringOrOld(requestData["price"], oldBook.getPrice());
    string pages = getStringOrOld(requestData["pages"], oldBook.getPages());
    string description = getStringOrOld(requestData["description"], oldBook.getDescription());

    Book book = Book(bookId, title, author, category, publisher, publishDate, price, pages, description);

    if (inventoryService->updateBook(book)) {
        json responseData = {
            {"success", true},
            {"message", "图书更新成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        res = HttpUtils::createErrorResponse("图书更新失败", 500);
    }
}

// POST /api/admin/books/delete - 删除图书
void AdminBookController::handleDeleteBook(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/admin/books/delete 删除图书");

    json requestData = HttpUtils::parseRequestBody(req);
    string bookId = requestData["isbn"];
    if (inventoryService->getAvailableCopyCount(bookId) != inventoryService->getBookCopyCount(bookId)) {
        res = HttpUtils::createErrorResponse("图书副本未全部归还", 400);
        return;
    }
    if (inventoryService->deleteBook(bookId) && inventoryService->deleteBookCopy(bookId)) {
        json responseData = {
            {"success", true},
            {"message", "图书删除成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        res = HttpUtils::createErrorResponse("图书删除失败", 500);
    }
}

//GET /api/admin/copies?isbn=<isbn> - 获取所有图书副本
void AdminBookController::handleGetAllBookCopies(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/copies 获取所有图书副本");

    if (!req.has_param("isbn")) {
        res = HttpUtils::createErrorResponse("缺少isbn参数", 400);
        return;
    }

    const string bookId = req.get_param_value("isbn");
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
}

//POST /api/admin/copies/add - 添加图书副本
void AdminBookController::handleAddBookCopy(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/admin/copies/add 添加图书副本");

    json requestData = HttpUtils::parseRequestBody(req);
    string bookId = requestData["isbn"];
    string copyId = inventoryService->generateCopyId(bookId);
    BookCopy bookCopy = BookCopy(bookId, copyId, "available");
    if (inventoryService->addBookCopy(bookCopy) && inventoryService->updateBookCopyCount(bookId)) {
        json responseData = {
            {"success", true},
            {"message", "图书副本添加成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        res = HttpUtils::createErrorResponse("图书副本添加失败", 500);
    }
}

//POST /api/admin/copies/delete - 删除图书副本
void AdminBookController::handleDeleteBookCopy(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/admin/copies/delete 删除图书副本");
 json requestData = HttpUtils::parseRequestBody(req);
    string copyId = requestData["copyId"];
    if (inventoryService->deleteBookCopy(copyId)) {
        json responseData = {
            {"success", true},
            {"message", "图书副本删除成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        res = HttpUtils::createErrorResponse("图书副本删除失败", 500);
    }
}
