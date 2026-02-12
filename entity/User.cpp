#include "User.h"
#include<string>
#include "../utils/PasswordUtils.h"
using namespace std;

User::User(string id, string name, string password, string type, int borrowedBookCount) {
    this->id = id;
    this->name = name;
    password = PasswordUtils::encryptPassword(password);
    this->password = password;
    this->type = type;
    this->borrowedBookCount = borrowedBookCount;
}

User::User() {
    this->id = "";
    this->name = "";
    this->password = "";
    this->type = "";
    this->borrowedBookCount = 0;
}

User::~User() = default;

//获取用户id
string User::getId() const {
    return id;
}

//获取用户名
string User::getName() const {
    return name;
}

//获取用户密码
string User::getPassword() const {
    return password;
}

//获取用户类型
string User::getType() const {
    return type;
}

int User::getBorrowedBookCount() const {
    return borrowedBookCount;
}

string User::getStatus() const {
    return status;
}

void User::setId(string id) {
    this->id = id;
}

void User::setName(string name) {
    this->name = name;
}

//设置用户密码
void User::setPassword(string password) {
    password = PasswordUtils::encryptPassword(password);
    this->password = password;
}

//设置用户类型
void User::setType(string type) {
    this->type = type;
}

void User::setBorrowedBookCount(int borrowedBookCount) {
    this->borrowedBookCount = borrowedBookCount;
}

void User::setStatus(string status) {
    this->status = status;
}
