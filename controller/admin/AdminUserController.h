#ifndef AVL_BOOKSYSTEM_ADMINUSERCONTROLLER_H
#define AVL_BOOKSYSTEM_ADMINUSERCONTROLLER_H
#include "service/UserService.h"
#include "service/BlackListService.h"
#include "service/BorrowService.h"
#include "service/InventoryService.h"
#include "HttpUtils.h"

//读者管理（查看读者信息，冻结/解冻账户）
class AdminUserController {
    UserService *userService;
    BlackListService *blackListService;
    BorrowService *borrowService;
    InventoryService *inventoryService;

public:
    explicit AdminUserController(UserService *userService, BlackListService *blackListService,
                                 BorrowService *borrowService, InventoryService *inventoryService);

    ~AdminUserController();

    //GET /api/admin/readers - 获取所有读者
    void handleGetAllUsers(httplib::Response &res) const;

    //GET /api/admin/readers/detail?userId=<userId> - 获取读者详细信息
    void handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const;

    //GET /api/admin/readers/search?userId=<userId> - 搜索读者
    void handleSearchUsers(const httplib::Request &req, httplib::Response &res) const;
};


#endif //AVL_BOOKSYSTEM_ADMINUSERCONTROLLER_H
