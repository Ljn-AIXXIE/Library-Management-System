#include "UserService.h"
#include <iostream>

UserService::UserService(UserDAO *db) : db(db) {
}

UserService::~UserService() = default;

bool UserService::registerUser(const User &user) const {
    return db->addUser(user);
}

bool UserService::loginUser(const string &userId, const string &password) const {
    if (verifyUser(userId, password)) {
        cout << "登录成功" << endl;
        return true;
    }
    cout << "登录失败" << endl;
    return false;
}

bool UserService::changePassword(const string &userId, const string &newPassword) const {
    if (db->updateUserPassword(userId, newPassword)) {
        cout << "密码修改成功" << endl;
        return true;
    }
    cout << "密码修改失败" << endl;
    return false;
}

bool UserService::deleteUser(const string &userId) const {
    return db->deleteUser(userId);
}

bool UserService::getUserById(const string &userId, User &user) const {
    return db->searchUserById(userId, user);
}

vector<User> UserService::getAllUsers() const {
    return db->getAllUsers();
}

// bool UserService::isUserBlacklisted(const string &userId) const {
//
// }

bool UserService::isUserExist(const string &userId) const {
    return db->exists(userId);
}

bool UserService::verifyUser(const string &userId, const string &password) const {
    return db->verifyUser(userId, password);
}

bool UserService::verifyOldPassword(const string &userId, const string &oldPassword) const {
    return db->verifyUser(userId, oldPassword);
}
