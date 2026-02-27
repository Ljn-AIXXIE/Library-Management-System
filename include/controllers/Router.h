#ifndef LIBRARY_MANAGEMENT_SYSTEM_ROUTER_H
#define LIBRARY_MANAGEMENT_SYSTEM_ROUTER_H

#include <httplib.h>
#include <memory>

#include "admin/AdminBatchAddController.h"
#include "admin/AdminBlackListController.h"
#include "admin/AdminBookController.h"
#include "admin/AdminBookExceptionController.h"
#include "admin/AdminController.h"
#include "admin/AdminUserController.h"
#include "user/AuthController.h"
#include "user/BookController.h"
#include "user/UserController.h"

//路由管理器 - 负责注册和管理所有HTTP路由
class Router {
    httplib::Server *server;
    std::unique_ptr<AuthController> authController;
    std::unique_ptr<BookController> bookSearchController;
    std::unique_ptr<UserController> userController;

    std::unique_ptr<AdminBookController> adminBookController;
    std::unique_ptr<AdminUserController> adminUserController;
    std::unique_ptr<AdminBlackListController> adminBlackListController;
    std::unique_ptr<AdminBatchAddController> adminBatchAddController;
    std::unique_ptr<AdminController> adminController;
    std::unique_ptr<AdminBookExceptionController> adminBookExceptionController;

    void registerAuthRoutes() const; //注册，登录，登出的路由
    void registerAdminBookRoutes() const; //注册管理员图书管理的路由
    void registerAdminUserRoutes() const; //注册管理员用户管理的路由
    void registerAdminBatchAddRoutes() const; //注册管理员批量添加数据的路由
    void registerAdminBookExceptionReportRoutes() const; //注册管理员书本异常处理的路由
    void registerBookRoutes() const; //注册图书相关的路由
    void registerProfileRoutes() const; //注册显示用户信息相关的路由

    //注册中间件
    void registerMiddleware() const;

    // CORS中间件
    void setupCORS() const;

public:
    explicit Router(httplib::Server *server) : server(server) {
    }

    // 初始化所有路由
    void initializeRoutes(UserService *userService, InventoryService *inventoryService, SearchService *searchService,
                          BlackListService *blackListService, BatchAddService *batchAddService,
                          BorrowService *borrowService, BookExceptionReportService *bookExceptionReportService);

    // 设置静态文件目录
    void setStaticFileDirectory(const std::string &dir) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_ROUTER_H
