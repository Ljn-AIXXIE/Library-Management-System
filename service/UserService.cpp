#include "UserService.h"
#include "../common/Logger.h"
#include <iostream>

UserService::UserService(UserDAO *db) : db(db) {
}

UserService::~UserService() = default;

bool UserService::registerUser(const User &user) const {
    Logger::getInstance().logBusiness("用户" + user.getId() + "注册");
    if (db->addUser(user)) {
        Logger::getInstance().logBusiness("用户" + user.getId() + "注册成功");
        return true;
    }
    Logger::getInstance().logError("用户" + user.getId() + "注册失败");
    return false;
}

bool UserService::loginUser(const string &userId, const string &password) const {
    if (verifyUser(userId, password)) {
        Logger::getInstance().logBusiness("用户" + userId + "登录成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "登录失败,密码错误");
    return false;
}

bool UserService::changePassword(const string &userId, const string &newPassword) const {
    if (db->updateUserPassword(userId, newPassword)) {
        Logger::getInstance().logBusiness("用户" + userId + "修改密码成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "修改密码失败");
    return false;
}

bool UserService::deleteUser(const string &userId) const {
    if (db->deleteUser(userId)) {
        Logger::getInstance().logBusiness("用户" + userId + "删除成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "删除失败");
    return false;
}

bool UserService::getUserById(const string &userId, User &user) const {
    return db->searchUserById(userId, user);
}

vector<User> UserService::getAllUsers() const {
    return db->getAllUsers();
}

bool UserService::isUserExist(const string &userId) const {
    return db->exists(userId);
}

bool UserService::verifyUser(const string &userId, const string &password) const {
    return db->verifyUser(userId, password);
}

bool UserService::verifyOldPassword(const string &userId, const string &oldPassword) const {
    return db->verifyUser(userId, oldPassword);
}
