#include "AdminBookController.h"

AdminBookController::AdminBookController(InventoryService *inventoryService, SearchService *searchService) : inventoryService(inventoryService) , searchService(searchService) {}

AdminBookController::~AdminBookController() = default;

void AdminBookController::handleAddBook(const httplib::Request &req, httplib::Response &res) const {
    json requestData = HttpUtils::parseRequestBody(req);

    // 验证必填字段
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"isbn", "title", "author", "category"}, errorMsg)) {
        cout << "添加图书失败: 缺少必填字段 - " << errorMsg << endl;
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    // 构建图书对象
    string bookId = requestData["isbn"];
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
        cout << "图书添加成功！" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        cout << "添加图书失败" << endl;
        res = HttpUtils::createErrorResponse("图书添加失败", 500);
    }
}

void AdminBookController::handleUpdateBook(const httplib::Request &req, httplib::Response &res) const {
    json requestData = HttpUtils::parseRequestBody(req);

    // 获取旧图书对象
    string bookId = requestData["isbn"];
    Book oldBook;
    if (!searchService->searchBookById(bookId, oldBook)) {
        cout << "更新图书失败: 图书不存在" << endl;
        res = HttpUtils::createErrorResponse("图书不存在", 404);
        return;
    }
    string title = requestData["title"].is_null() ? oldBook.getTitle() : requestData["title"].get<string>();
    string author = requestData["author"].is_null() ? oldBook.getAuthor() : requestData["author"].get<string>();
    string category = requestData["category"].is_null() ? oldBook.getCategory() : requestData["category"].get<string>();
    string publisher = requestData["publisher"].is_null() ? oldBook.getPublisher() : requestData["publisher"].get<string>();
    string publishDate = requestData["publishDate"].is_null() ? oldBook.getPublishDate() : requestData["publishDate"].get<string>();
    string price = requestData["price"].is_null() ? oldBook.getPrice() : requestData["price"].get<string>();
    string pages = requestData["pages"].is_null() ? oldBook.getPages() : requestData["pages"].get<string>();
    string description = requestData["description"].is_null() ? oldBook.getDescription() : requestData["description"].get<string>();

    Book book = Book(bookId, title, author, category, publisher, publishDate, price, pages, description);

    if (inventoryService->updateBook(book)) {
        json responseData = {
            {"success", true},
            {"message", "图书更新成功"}
        };
        cout << "图书更新成功" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        cout<<"更新图书失败: 未知错误"<<endl;
        res = HttpUtils::createErrorResponse("图书更新失败", 500);
    }
}

void AdminBookController::handleDeleteBook(const httplib::Request &req, httplib::Response &res) const {
    json requestData = HttpUtils::parseRequestBody(req);
    string bookId = requestData["isbn"];
    if (inventoryService->deleteBook(bookId)) {
        json responseData = {
            {"success", true},
            {"message", "图书删除成功"}
        };
        cout << "图书删除成功" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        cout << "删除图书失败: 未知错误" << endl;
        res = HttpUtils::createErrorResponse("图书删除失败", 500);
    }
}