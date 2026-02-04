#ifndef AVL_BOOKSYSTEM_ROUTER_H
#define AVL_BOOKSYSTEM_ROUTER_H

#include "../utils/httplib.h"
#include "user/AuthController.h"
#include "admin/AdminBookController.h"
#include "admin/AdminUserController.h"
#include <memory>

using namespace std;

//路由管理器 - 负责注册和管理所有HTTP路由
class Router {
    httplib::Server* server;
    unique_ptr<AuthController> authController;
    unique_ptr<AdminBookController> adminBookController;
    unique_ptr<AdminUserController> adminUserController;

    void registerAuthRoutes() const;//注册，登录，登出的路由
    void registerAdminBookRoutes() const;//注册管理员图书管理的路由
    void registerAdminUserRoutes() const;//注册管理员用户管理的路由

    //注册中间件
    void registerMiddleware() const;
    
    // CORS中间件
    void setupCORS() const;

public:
    explicit Router(httplib::Server* server);
    ~Router();

    // 初始化所有路由
    void initializeRoutes(UserService* userService, InventoryService* inventoryService, SearchService* searchService);

    // 设置静态文件目录
    void setStaticFileDirectory(const string& dir) const;
};

#endif //AVL_BOOKSYSTEM_ROUTER_H

