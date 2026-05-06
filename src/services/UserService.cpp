#include "services/UserService.h"

#include "common/Logger.h"

namespace {
    constexpr char kBootstrapSuperAdminId[] = "000000000001";
    constexpr char kBootstrapSuperAdminName[] = "超级管理员";
    constexpr char kBootstrapSuperAdminPassword[] = "123456";
} // namespace

void UserService::ensureBootstrapSuperAdmin() const {
    if (db->countUsersWithRole("super_admin") > 0) {
        return;
    }
    std::string errorMessage;
    if (db->exists(kBootstrapSuperAdminId)) {
        if (db->updateUserRole(kBootstrapSuperAdminId, "super_admin", errorMessage)) {
            Logger::getInstance().logBusiness(
                std::string("已将既有账号 ") + kBootstrapSuperAdminId + " 升级为超级管理员");
        } else {
            Logger::getInstance().logError("升级超级管理员失败: " + errorMessage);
        }
        return;
    }
    const User bootstrapUser(kBootstrapSuperAdminId, kBootstrapSuperAdminName, kBootstrapSuperAdminPassword,
                             "super_admin");
    if (!db->addUser(bootstrapUser, errorMessage)) {
        Logger::getInstance().logError("首次部署创建超级管理员失败: " + errorMessage);
        return;
    }
    Logger::getInstance().logBusiness(
        std::string("首次部署：已创建默认超级管理员账号 ") + kBootstrapSuperAdminId + "（请尽快登录并修改密码）");
}

bool UserService::isSuperAdministrator(const std::string &userId) const {
    User user;
    if (!db->searchUserById(userId, user)) {
        return false;
    }
    return user.getType() == "super_admin";
}

bool UserService::superAdminListManagers(const std::string &operatorUserId,
                                         std::vector<UserDAO::AdministratorRecord> &out,
                                         std::string &errorMessage) const {
    if (!isSuperAdministrator(operatorUserId)) {
        errorMessage = "需要超级管理员权限";
        return false;
    }
    out = db->listAdministratorAccounts();
    return true;
}

bool UserService::superAdminPromoteStudentToAdmin(const std::string &operatorUserId,
                                                  const std::string &targetUserId,
                                                  std::string &errorMessage) const {
    if (!isSuperAdministrator(operatorUserId)) {
        errorMessage = "需要超级管理员权限";
        return false;
    }
    User target;
    if (!db->searchUserById(targetUserId, target)) {
        errorMessage = "用户不存在";
        return false;
    }
    if (target.getType() != "student") {
        errorMessage = "只能将读者（student）账号提拔为管理员";
        return false;
    }
    if (!db->updateUserRole(targetUserId, "admin", errorMessage)) {
        return false;
    }
    Logger::getInstance().logBusiness("超级管理员 " + operatorUserId + " 将读者 " + targetUserId + " 设为管理员");
    return true;
}

bool UserService::superAdminDemoteAdminToStudent(const std::string &operatorUserId,
                                                 const std::string &targetUserId,
                                                 std::string &errorMessage) const {
    if (!isSuperAdministrator(operatorUserId)) {
        errorMessage = "需要超级管理员权限";
        return false;
    }
    User target;
    if (!db->searchUserById(targetUserId, target)) {
        errorMessage = "用户不存在";
        return false;
    }
    if (target.getType() == "super_admin") {
        errorMessage = "不能撤销超级管理员身份";
        return false;
    }
    if (target.getType() != "admin") {
        errorMessage = "该用户不是管理员，无法撤销";
        return false;
    }
    if (!db->updateUserRole(targetUserId, "student", errorMessage)) {
        return false;
    }
    Logger::getInstance().logBusiness("超级管理员 " + operatorUserId + " 撤销管理员 " + targetUserId + "，已恢复为读者");
    return true;
}

bool UserService::superAdminAddAdmin(const std::string &operatorUserId, const std::string &targetUserId,
                                     const std::string &targetUserName,
                                     std::string &errorMessage) const {
    if (!isSuperAdministrator(operatorUserId)) {
        errorMessage = "需要超级管理员权限";
        return false;
    }
    if (targetUserId.length() != 12) {
        errorMessage = "用户ID长度必须为12个字符";
        return false;
    }
    if (targetUserId == kBootstrapSuperAdminId) {
        errorMessage = "该用户ID为系统保留，无法用于新建管理员";
        return false;
    }
    if (isUserExist(targetUserId)) {
        errorMessage = "该账号已被占用，请换一个账户号";
        return false;
    }

    const User newUser(targetUserId, targetUserName, "123456", "admin");
    if (!db->addUser(newUser, errorMessage)) {
        return false;
    }
    Logger::getInstance().logBusiness(
        "超级管理员" + operatorUserId + "添加新管理员:" + targetUserId + "，初始密码为：123456，请尽快登录修改密码");
    return true;
}

bool UserService::superAdminDeleteAdmin(const std::string &operatorUserId, const std::string &targetUserId,
                                        std::string &errorMessage) const {
    if (!isSuperAdministrator(operatorUserId)) {
        errorMessage = "需要超级管理员权限";
        return false;
    }
    if (targetUserId == operatorUserId) {
        errorMessage = "不能删除当前登录账号";
        return false;
    }
    User target;
    if (!db->searchUserById(targetUserId, target)) {
        errorMessage = "该账号不存在，请重新检查";
        return false;
    }
    if (target.getType() == "super_admin") {
        errorMessage = "不能删除超级管理员账号";
        return false;
    }
    if (target.getType() != "admin") {
        errorMessage = "只能删除角色为管理员（admin）的账号";
        return false;
    }
    if (!db->deleteUser(targetUserId, errorMessage)) {
        return false;
    }
    Logger::getInstance().logBusiness("超级管理员" + operatorUserId + "删除管理员账号:" + targetUserId);
    return true;
}

bool UserService::registerUser(const User &user) const {
    Logger::getInstance().logBusiness("用户" + user.getId() + "注册");
    std::string errorMessage;
    if (db->addUser(user, errorMessage)) {
        Logger::getInstance().logBusiness("用户" + user.getId() + "注册成功");
        return true;
    }
    Logger::getInstance().logError("用户" + user.getId() + "注册失败");
    return false;
}

bool UserService::loginUser(const std::string &userId, const std::string &password) const {
    if (verifyUser(userId, password)) {
        Logger::getInstance().logBusiness("用户" + userId + "登录成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "登录失败,密码错误");
    return false;
}

bool UserService::changePassword(const std::string &userId, const std::string &newPassword) const {
    if (db->updateUserPassword(userId, newPassword)) {
        Logger::getInstance().logBusiness("用户" + userId + "修改密码成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "修改密码失败");
    return false;
}

bool UserService::deleteUser(const std::string &userId, std::string &errorMessage) const {
    if (db->deleteUser(userId, errorMessage)) {
        Logger::getInstance().logBusiness("用户" + userId + "删除成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "删除失败");
    return false;
}

bool UserService::getUserById(const std::string &userId, User &user) const {
    return db->searchUserById(userId, user);
}

std::vector<User> UserService::getAllUsers() const {
    return db->getAllUsers();
}

int UserService::getTotalUserCount() const {
    return db->getTotalUserCount();
}

bool UserService::isUserExist(const std::string &userId) const {
    return db->exists(userId);
}

bool UserService::verifyUser(const std::string &userId, const std::string &password) const {
    return db->verifyUser(userId, password);
}

bool UserService::verifyOldPassword(const std::string &userId, const std::string &oldPassword) const {
    return db->verifyUser(userId, oldPassword);
}
