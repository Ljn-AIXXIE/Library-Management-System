#ifndef LIBRARY_MANAGEMENT_SYSTEM_AUTHCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_AUTHCONTROLLER_H

#include <httplib.h>
#include <json.hpp>

#include "services/UserService.h"

using json = nlohmann::json;

// 认证控制器 - 处理登录、注册、登出等认证相关操作
class AuthController {
    UserService *userService;
public:
    explicit AuthController(UserService *userService) : userService(userService) {}

    // POST /api/auth/register - 用户注册
    void handleRegister(const httplib::Request &req, httplib::Response &res) const;

    // POST /api/auth/login - 用户登录
    void handleLogin(const httplib::Request &req, httplib::Response &res) const;

    // POST /api/auth/logout - 用户登出
    static void handleLogout(const httplib::Request &req, httplib::Response &res);

    // POST /api/auth/change-password - 修改密码
    void handleChangePassword(const httplib::Request &req, httplib::Response &res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_AUTHCONTROLLER_H

