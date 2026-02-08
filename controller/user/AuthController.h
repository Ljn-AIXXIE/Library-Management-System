#ifndef AVL_BOOKSYSTEM_AUTHCONTROLLER_H
#define AVL_BOOKSYSTEM_AUTHCONTROLLER_H

#include "../../utils/httplib.h"
#include "../../service/UserService.h"
#include "../../utils/PasswordUtils.h"
#include "../../utils/json.hpp"
#include "../../utils/HttpUtils.h"
#include <string>
#include <iostream>

using json = nlohmann::json;
using namespace std;

// 认证控制器 - 处理登录、注册、登出等认证相关操作
class AuthController {
    UserService* userService;

public:
    explicit AuthController(UserService* userService);
    ~AuthController();

    // POST /api/auth/register - 用户注册
    void handleRegister(const httplib::Request& req, httplib::Response& res) const;
    
    // POST /api/auth/login - 用户登录
    void handleLogin(const httplib::Request& req, httplib::Response& res) const;
    
    // POST /api/auth/logout - 用户登出
    static void handleLogout(const httplib::Request& req, httplib::Response& res);

    // POST /api/auth/change-password - 修改密码
    void handleChangePassword(const httplib::Request& req, httplib::Response& res) const;
};

#endif //AVL_BOOKSYSTEM_AUTHCONTROLLER_H

