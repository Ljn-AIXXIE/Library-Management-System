#ifndef LIBRARY_MANAGEMENT_SYSTEM_SUPERADMINCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_SUPERADMINCONTROLLER_H

#include <httplib.h>

#include "services/UserService.h"

class SuperAdminController {
    UserService *userService;

public:
    explicit SuperAdminController(UserService *userService) : userService(userService) {}

    void handleListManagers(const httplib::Request &req, httplib::Response &res) const;
    void handlePromote(const httplib::Request &req, httplib::Response &res) const;
    void handleDemote(const httplib::Request &req, httplib::Response &res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_SUPERADMINCONTROLLER_H
