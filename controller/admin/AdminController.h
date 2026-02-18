#ifndef LIBRARYMANAGEMENTSYSTEM_ADMINCONTROLLER_H
#define LIBRARYMANAGEMENTSYSTEM_ADMINCONTROLLER_H
#include "../service/UserService.h"
#include "../service/InventoryService.h"
#include "../service/BorrowService.h"
#include "../utils/httplib.h"

class AdminController {
    UserService *userService;
    InventoryService *inventoryService;
    BorrowService *borrowService;

public:
    AdminController(UserService *userService, InventoryService *inventoryService, BorrowService *borrowService);

    ~AdminController() = default;

    //POST /api/admin/stats - 获取系统统计信息
    void handleGetStats(httplib::Response &res) const;
};


#endif //LIBRARYMANAGEMENTSYSTEM_ADMINCONTROLLER_H
