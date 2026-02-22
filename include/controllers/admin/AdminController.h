#ifndef LIBRARY_MANAGEMENT_SYSTEM_ADMINCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_ADMINCONTROLLER_H

#include <httplib.h>

#include "services/BorrowService.h"
#include "services/InventoryService.h"
#include "services/UserService.h"

class AdminController {
    UserService *userService;
    InventoryService *inventoryService;
    BorrowService *borrowService;

public:
    AdminController(UserService *userService, InventoryService *inventoryService, BorrowService *borrowService)
        : userService(userService), inventoryService(inventoryService), borrowService(borrowService) {}

    //GET /api/admin/stats - 获取系统统计信息
    void handleGetStats(httplib::Response &res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_ADMINCONTROLLER_H
