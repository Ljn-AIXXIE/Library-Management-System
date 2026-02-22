#ifndef LIBRARY_MANAGEMENT_SYSTEM_USERCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_USERCONTROLLER_H

#include "services/BorrowService.h"
#include "utils/HttpUtils.h"

class UserController {
    BorrowService *borrowService;
public:
    explicit UserController(BorrowService *borrowService) : borrowService(borrowService) {}

    //GET /api/reader/stats - 获取用户信息
    void handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_USERCONTROLLER_H
