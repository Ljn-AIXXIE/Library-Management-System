#ifndef LIBRARY_MANAGEMENT_SYSTEM_SUPERADMINCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_SUPERADMINCONTROLLER_H

#include <httplib.h>

#include "services/UserService.h"

class SuperAdminController {
    UserService *userService;

public:
    explicit SuperAdminController(UserService *userService) : userService(userService) {
    }

    //列出所有的管理员（包括超级管理员）
    void handleListManagers(const httplib::Request &req, httplib::Response &res) const;

    //将普通读者提拔为管理员账号
    void handlePromote(const httplib::Request &req, httplib::Response &res) const;

    //将管理员账号降级为普通读者账号
    void handleDemote(const httplib::Request &req, httplib::Response &res) const;

    //添加管理员
    void handleAddAdmin(const httplib::Request &req, httplib::Response &res) const;

    //删除管理员账号
    void handleDeleteAdmin(const httplib::Request &req, httplib::Response &res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_SUPERADMINCONTROLLER_H
