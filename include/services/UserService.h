#ifndef LIBRARY_MANAGEMENT_SYSTEM_USERSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_USERSERVICE_H

#include <string>
#include <vector>

#include "database/dao/UserDAO.h"

//用于管理用户，包括用户注册、登录、修改密码等操作
class UserService {
    UserDAO *db;

public:
    explicit UserService(UserDAO *db) : db(db) {
    }

    //核心操作
    [[nodiscard]] bool registerUser(const User &user) const; //用户注册
    [[nodiscard]] bool loginUser(const std::string &userId, const std::string &password) const; //用户登录
    [[nodiscard]] bool changePassword(const std::string &userId, const std::string &newPassword) const; //修改密码
    [[nodiscard]] bool deleteUser(const std::string &userId, std::string &errorMessage) const; //删除用户

    // 首次部署：若无超级管理员则创建默认账号（见实现内常量）
    void ensureBootstrapSuperAdmin() const;

    [[nodiscard]] bool isSuperAdministrator(const std::string &userId) const;

    [[nodiscard]] bool superAdminListManagers(const std::string &operatorUserId,
                                              std::vector<UserDAO::AdministratorRecord> &out,
                                              std::string &errorMessage) const;

    [[nodiscard]] bool superAdminPromoteStudentToAdmin(const std::string &operatorUserId,
                                                       const std::string &targetUserId,
                                                       std::string &errorMessage) const;

    [[nodiscard]] bool superAdminDemoteAdminToStudent(const std::string &operatorUserId,
                                                      const std::string &targetUserId,
                                                      std::string &errorMessage) const;

    [[nodiscard]] bool superAdminAddAdmin(const std::string &operatorUserId,
                                          const std::string &targetUserId, const std::string &targetUserName,
                                          std::string &errorMessage) const;

    [[nodiscard]] bool superAdminDeleteAdmin(const std::string &operatorUserId,
                                             const std::string &targetUserId,
                                             std::string &errorMessage) const;

    //查询
    bool getUserById(const std::string &userId, User &user) const; //根据用户ID获取用户信息
    [[nodiscard]] std::vector<User> getAllUsers() const;

    [[nodiscard]] int getTotalUserCount() const;

    //校验
    [[nodiscard]] bool isUserExist(const std::string &userId) const; //用户是否存在
    [[nodiscard]] bool verifyUser(const std::string &userId, const std::string &password) const; //验证用户登录
    [[nodiscard]] bool verifyOldPassword(const std::string &userId, const std::string &oldPassword) const; //验证旧密码
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_USERSERVICE_H
