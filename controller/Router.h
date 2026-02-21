#ifndef AVL_BOOKSYSTEM_ROUTER_H
#define AVL_BOOKSYSTEM_ROUTER_H

#include "utils/httplib.h"
#include "user/AuthController.h"
#include "user/UserController.h"
#include "user/BookController.h"

#include "admin/AdminBookController.h"
#include "admin/AdminUserController.h"
#include "admin/AdminBlackListController.h"
// #include "admin/AdminBatchAddController.h"
#include "admin/AdminController.h"
#include <memory>
using std::unique_ptr;


//路由管理器 - 负责注册和管理所有HTTP路由
class Router {
    httplib::Server *server;
    unique_ptr<AuthController> authController;
    unique_ptr<BookController> bookSearchController;
    unique_ptr<UserController> userController;

    unique_ptr<AdminBookController> adminBookController;
    unique_ptr<AdminUserController> adminUserController;
    unique_ptr<AdminBlackListController> adminBlackListController;
    // unique_ptr<AdminBatchAddController> adminBatchAddController;
    unique_ptr<AdminController> adminController;

    void registerAuthRoutes() const; //注册，登录，登出的路由
    void registerAdminBookRoutes() const; //注册管理员图书管理的路由
    void registerAdminUserRoutes() const; //注册管理员用户管理的路由
    // void registerAdminBatchAddRoutes() const; //注册管理员批量添加数据的路由
    void registerBookRoutes() const; //注册图书相关的路由
    void registerProfileRoutes() const; //注册显示用户信息相关的路由

    //注册中间件
    void registerMiddleware() const;

    // CORS中间件
    void setupCORS() const;

public:
    explicit Router(httplib::Server *server);

    ~Router();

    // 初始化所有路由
    void initializeRoutes(UserService *userService, InventoryService *inventoryService, SearchService *searchService,
                          BlackListService *blackListService, BorrowService *borrowService);

    // 设置静态文件目录
    void setStaticFileDirectory(const string &dir) const;
};

#endif //AVL_BOOKSYSTEM_ROUTER_H

