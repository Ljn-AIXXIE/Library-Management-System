#ifndef LIBRARY_MANAGEMENT_SYSTEM_USERDAO_H
#define LIBRARY_MANAGEMENT_SYSTEM_USERDAO_H

#include <string>
#include <vector>

#include "database/DatabaseOperator.h"
#include "entities/User.h"

class UserDAO {
    DatabaseOperator *userDatabase;

public:
    explicit UserDAO(DatabaseOperator *userDatabase) : userDatabase(userDatabase) {
    }

    struct AdministratorRecord {
        std::string userId;
        std::string name;
        std::string role;
    };

    //核心操作
    [[nodiscard]] bool addUser(const User &user, std::string &errorMessage) const; //添加用户
    [[nodiscard]] bool deleteUser(const std::string &userId, std::string &errorMessage) const; //删除用户

    [[nodiscard]] bool updateUserPassword(const std::string &userId, const std::string &newPassword) const; //更新用户密码
    [[nodiscard]] bool updateUserBorrowInfo(const std::string &userId, bool flag) const; //更新用户借阅信息
    [[nodiscard]] bool updateUserName(const std::string &userId, const std::string &newName) const; //更新用户名

    [[nodiscard]] bool searchUserById(const std::string &userId, User &user) const; //根据用户id查询用户信息
    [[nodiscard]] std::vector<User> getAllUsers() const; //获取所有用户信息，用于管理员查询所有用户

    [[nodiscard]] bool exists(const std::string &userId) const; //判断用户是否存在，用于登录和注册校验
    [[nodiscard]] bool verifyUser(const std::string &userId, const std::string &password) const; //验证用户登录
    [[nodiscard]] int getBorrowedBookCount(const std::string &userId) const; //获取用户已借阅数量
    [[nodiscard]] int getTotalUserCount() const; //获取用户总数

    [[nodiscard]] int countUsersWithRole(const std::string &role) const;

    [[nodiscard]] bool updateUserRole(const std::string &userId, const std::string &newRole,
                                      std::string &errorMessage) const;

    [[nodiscard]] std::vector<AdministratorRecord> listAdministratorAccounts() const;

    //std::vector<User> getAllUsers();//获取所有用户信息，用于管理员查询所有用户
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_USERDAO_H
