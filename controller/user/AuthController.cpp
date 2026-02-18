#include "AuthController.h"
#include "../common/Logger.h"
using std::string;

AuthController::AuthController(UserService *userService) : userService(userService) {
}

AuthController::~AuthController() = default;

// POST /api/auth/register - 用户注册
void AuthController::handleRegister(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/auth/register 用户注册");

    json requestData = HttpUtils::parseRequestBody(req);
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"userId", "name", "password"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    string userId = requestData["userId"];
    string name = requestData["name"];
    string password = requestData["password"];
    // 普通用户注册时强制设置为 student，不允许用户自己选择角色
    string role = "student";

    // 验证用户ID格式（必须为12个字符）
    if (userId.length() != 12) {
        res = HttpUtils::createErrorResponse("用户ID长度必须为12个字符", 400);
        return;
    }

    // 验证密码强度
    if (password.length() < 6) {
        res = HttpUtils::createErrorResponse("密码长度至少为6个字符", 400);
        return;
    }

    // 检查用户是否已存在
    if (userService->isUserExist(userId)) {
        res = HttpUtils::createErrorResponse("用户已存在", 409);
        return;
    }

    // 创建用户对象（User构造函数会自动加密密码）
    User newUser(userId, name, password, role);

    // 注册用户
    if (userService->registerUser(newUser)) {
        json responseData = {
            {"success", true},
            {"message", "注册成功"},
            {
                "data", {
                    {"userId", userId},
                    {"name", name},
                    {"role", role}
                }
            }
        };
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        res = HttpUtils::createErrorResponse("注册失败，请稍后重试", 500);
    }
}

// POST /api/auth/login - 用户登录
void AuthController::handleLogin(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/auth/login 用户登录");

    json requestData = HttpUtils::parseRequestBody(req);
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"userId", "password"}, errorMsg)) {
        std::cout << "登录失败: 缺少必填字段 - " << errorMsg << std::endl;
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    string userId = requestData["userId"];
    string password = requestData["password"];

    // 检查用户是否存在
    if (!userService->isUserExist(userId)) {
        res = HttpUtils::createErrorResponse("用户不存在", 401);
        return;
    }

    // 验证用户登录
    if (userService->loginUser(userId, password)) {
        // 获取用户信息
        User user;
        if (userService->getUserById(userId, user)) {
            json responseData = {
                {"success", true},
                {"message", "登录成功"},
                {
                    "data", {
                        {"userId", user.getId()},
                        {"name", user.getName()},
                        {"role", user.getType()}
                    }
                }
            };
            res = HttpUtils::createSuccessResponse(responseData, 200);
        } else {
            Logger::getInstance().logError("用户登录成功但获取用户信息失败");
            res = HttpUtils::createErrorResponse("获取用户信息失败", 500);
        }
    } else {
        res = HttpUtils::createErrorResponse("用户ID或密码错误", 401);
    }
}

// POST /api/auth/logout - 用户登出
void AuthController::handleLogout(const httplib::Request &req, httplib::Response &res) {
    Logger::getInstance().logAccess("POST /api/auth/logout 用户登出");

    json responseData = {
        {"success", true},
        {"message", "登出成功"}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

// POST /api/auth/change-password - 修改密码
void AuthController::handleChangePassword(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/auth/change-password 修改密码");

    json requestData = HttpUtils::parseRequestBody(req);
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"userId", "oldPassword", "newPassword"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    string userId = requestData["userId"];
    string oldPassword = requestData["oldPassword"];
    string newPassword = requestData["newPassword"];

    // 验证用户是否存在
    if (!userService->isUserExist(userId)) {
        res = HttpUtils::createErrorResponse("用户不存在", 401);
        return;
    }

    // 验证旧密码
    if (!userService->verifyOldPassword(userId, oldPassword)) {
        res = HttpUtils::createErrorResponse("原密码错误", 401);
        return;
    }

    newPassword = PasswordUtils::encryptPassword(newPassword);

    // 修改密码
    if (userService->changePassword(userId, newPassword)) {
        json responseData = {
            {"success", true},
            {"message", "密码修改成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        res = HttpUtils::createErrorResponse("密码修改失败", 500);
    }
}
