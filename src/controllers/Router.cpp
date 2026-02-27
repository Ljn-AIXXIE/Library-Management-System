#include "controllers/Router.h"

#include <iostream>
#include <json.hpp>

using std::make_unique;

// 初始化所有路由
void Router::initializeRoutes(UserService *userService, InventoryService *inventoryService,
                              SearchService *searchService, BlackListService *blackListService,
                              BatchAddService *batchAddService, BorrowService *borrowService,
                              BookExceptionReportService *bookExceptionReportService) {
    // 创建Controller实例
    authController = make_unique<AuthController>(userService);
    adminBookController = make_unique<AdminBookController>(inventoryService, searchService, borrowService, userService);
    adminUserController = make_unique<AdminUserController>(userService, blackListService, borrowService,
                                                           inventoryService);
    adminBlackListController = make_unique<AdminBlackListController>(blackListService);
    adminBatchAddController = make_unique<AdminBatchAddController>(batchAddService);
    bookSearchController = make_unique<
        BookController>(searchService, inventoryService, borrowService, blackListService);
    userController = make_unique<UserController>(borrowService);
    adminController = make_unique<AdminController>(userService, inventoryService, borrowService);
    adminBookExceptionController = make_unique<AdminBookExceptionController>(
        bookExceptionReportService, inventoryService);

    // 设置CORS
    setupCORS();

    // 注册中间件
    registerMiddleware();

    // 注册路由
    registerAuthRoutes();
    registerAdminBookRoutes();
    registerAdminUserRoutes();
    registerAdminBatchAddRoutes();
    registerAdminBookExceptionReportRoutes();
    registerBookRoutes();
    registerProfileRoutes();
}

// 注册，登录，登出的路由
void Router::registerAuthRoutes() const {
    // POST /api/auth/register - 用户注册
    server->Post("/api/auth/register", [this](const httplib::Request &req, httplib::Response &res) {
        authController->handleRegister(req, res);
    });

    // POST /api/auth/login - 用户登录
    server->Post("/api/auth/login", [this](const httplib::Request &req, httplib::Response &res) {
        authController->handleLogin(req, res);
    });

    // POST /api/auth/logout - 用户登出
    server->Post("/api/auth/logout", [this](const httplib::Request &req, httplib::Response &res) {
        authController->handleLogout(req, res);
    });

    // POST /api/auth/change_password - 修改密码
    server->Post("/api/auth/change-password", [this](const httplib::Request &req, httplib::Response &res) {
        authController->handleChangePassword(req, res);
    });
}

//注册管理员图书管理的路由
void Router::registerAdminBookRoutes() const {
    //GET /api/admin/books - 获取所有图书
    server->Get("/api/admin/books", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleGetAllBooks(res);
    });

    //POST /api/admin/books/add - 添加图书
    server->Post("/api/admin/books/add", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleAddBook(req, res);
    });

    //PUT /api/admin/books/update - 更新图书
    server->Put("/api/admin/books/update", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleUpdateBook(req, res);
    });

    //POST /api/admin/books/delete - 删除图书
    server->Post("/api/admin/books/delete", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleDeleteBook(req, res);
    });

    //GET /api/admin/copies?isbn=<isbn> - 获取所有图书副本
    server->Get("/api/admin/copies", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleGetAllBookCopies(req, res);
    });

    //POST /api/admin/copies/add - 添加图书副本
    server->Post("/api/admin/copies/add", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleAddBookCopy(req, res);
    });

    //POST /api/admin/copies/delete - 删除图书副本
    server->Post("/api/admin/copies/delete", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleDeleteBookCopy(req, res);
    });

    //POST /api/admin/borrow/manual - 人工借书
    server->Post("/api/admin/borrow/manual", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleManualBorrowBookCopy(req, res);
    });

    //POST /api/admin/return/manual - 人工还书
    server->Post("/api/admin/return/manual", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookController->handleManualReturnBookCopy(req, res);
    });
}

//注册管理员用户管理的路由
void Router::registerAdminUserRoutes() const {
    //GET /api/admin/readers - 获取所有读者
    server->Get("/api/admin/readers", [this](const httplib::Request &req, httplib::Response &res) {
        adminUserController->handleGetAllUsers(res);
    });

    //GET /api/admin/readers/detail?userId=<userId> - 获取读者详细信息
    server->Get("/api/admin/readers/detail", [this](const httplib::Request &req, httplib::Response &res) {
        adminUserController->handleGetUserDetail(req, res);
    });

    //POST /api/admin/readers/freeze - 冻结读者账户
    server->Post("/api/admin/readers/freeze", [this](const httplib::Request &req, httplib::Response &res) {
        adminBlackListController->handleFreezeUser(req, res);
    });

    //POST /api/admin/readers/unfreeze - 解冻读者账户
    server->Post("/api/admin/readers/unfreeze", [this](const httplib::Request &req, httplib::Response &res) {
        adminBlackListController->handleUnfreezeUser(req, res);
    });
}

void Router::registerAdminBatchAddRoutes() const {
    //POST /api/admin/add/batch - 批量添加数据
    server->Post("/api/admin/add/batch",
                 [this](const httplib::Request &req, httplib::Response &res,
                        const httplib::ContentReader &content_reader) {
                     adminBatchAddController->handleBatchAdd(req, res, content_reader);
                 });
}

void Router::registerAdminBookExceptionReportRoutes() const {
    //POST /api/exception/handle - 提交异常
    server->Post("/api/exception/submit", [this](const httplib::Request &req, httplib::Response &res) {
        adminBookExceptionController->handleAddException(req, res);
    });
}

void Router::registerBookRoutes() const {
    //GET /api/books/search - 搜索图书
    server->Get("/api/books/search", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleSearchBooks(req, res);
    });

    //GET /api/books/detail/book?isbn=<isbn> - 获取图书详细信息
    server->Get("/api/books/detail/book", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleGetBookDetail(req, res);
    });

    //GET /api/books/detail/copies?isbn=<isbn> - 获取图书副本信息
    server->Get("/api/books/detail/copies", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleGetBookCopies(req, res);
    });

    //POST /api/borrow - 借阅图书
    server->Post("/api/borrow", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleBorrowBook(req, res);
    });

    //GET /api/borrow/current - 获取用户当前借阅的记录
    server->Get("/api/borrow/current", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleGetUserBorrowingRecords(req, res);
    });

    //GET /api/borrow/history - 获取用户借阅历史
    server->Get("/api/borrow/history", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleGetUserBorrowHistory(req, res);
    });

    //POST /api/borrow/return - 归还图书
    server->Post("/api/borrow/return", [this](const httplib::Request &req, httplib::Response &res) {
        bookSearchController->handleReturnBook(req, res);
    });
}

//注册显示信息相关的路由
void Router::registerProfileRoutes() const {
    //GET /api/reader/stats - 获取用户信息
    server->Get("/api/reader/stats", [this](const httplib::Request &req, httplib::Response &res) {
        userController->handleGetUserDetail(req, res);
    });

    //GET /api/admin/stats - 获取管理员统计信息
    server->Get("/api/admin/stats", [this](const httplib::Request &req, httplib::Response &res) {
        adminController->handleGetStats(res);
    });
}

// 注册中间件
void Router::registerMiddleware() const {
    // 请求日志中间件
    server->set_logger([](const httplib::Request &req, const httplib::Response &res) {
        std::cout << "[" << req.method << "] " << req.path;
        if (!req.body.empty()) {
            std::cout << " - Body: " << req.body.substr(0, 100); // 只打印前100个字符
        }
        std::cout << " - Status: " << res.status << std::endl;
    });

    // 错误处理中间件
    server->set_error_handler([](const httplib::Request &req, httplib::Response &res) {
        // 如果响应已经有内容了，不要覆盖
        if (!res.body.empty()) {
            return;
        }

        nlohmann::json errorJson = {
            {"success", false},
            {"message", "请求处理失败"},
            {"status", res.status}
        };
        res.set_content(errorJson.dump(), "application/json; charset=utf-8");
    });

    std::cout << "✓ 中间件注册完成" << std::endl;
}

// 设置CORS
void Router::setupCORS() const {
    server->set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        // 设置CORS头
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");

        // 处理OPTIONS预检请求
        if (req.method == "OPTIONS") {
            res.status = 200;
            return httplib::Server::HandlerResponse::Handled;
        }

        return httplib::Server::HandlerResponse::Unhandled;
    });

    std::cout << "✓ CORS配置完成" << std::endl;
}

// 设置静态文件目录
void Router::setStaticFileDirectory(const std::string &dir) const {
    server->set_mount_point("/", dir);
    std::cout << "✓ 静态文件目录设置为: " << dir << std::endl;
}
