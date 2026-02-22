#ifndef LIBRARY_MANAGEMENT_SYSTEM_ADMINUSERCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_ADMINUSERCONTROLLER_H

#include "services/BlackListService.h"
#include "services/BorrowService.h"
#include "services/InventoryService.h"
#include "services/UserService.h"
#include "utils/HttpUtils.h"

//读者管理（查看读者信息，冻结/解冻账户）
class AdminUserController {
    UserService *userService;
    BlackListService *blackListService;
    BorrowService *borrowService;
    InventoryService *inventoryService;
public:
    AdminUserController(UserService *userService, BlackListService *blackListService,
                                 BorrowService *borrowService, InventoryService *inventoryService)
        : userService(userService), blackListService(blackListService), borrowService(borrowService), inventoryService(inventoryService) {}

    //GET /api/admin/readers - 获取所有读者
    void handleGetAllUsers(httplib::Response &res) const;

    //GET /api/admin/readers/detail?userId=<userId> - 获取读者详细信息
    void handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/admin/readers/search?userId=<userId> - 搜索读者
    void handleSearchUsers(const httplib::Request &req, httplib::Response &res) const;
};


#endif //LIBRARY_MANAGEMENT_SYSTEM_ADMINUSERCONTROLLER_H
