#ifndef LIBRARYMANAGEMENTSYSTEM_USERCONTROLLER_H
#define LIBRARYMANAGEMENTSYSTEM_USERCONTROLLER_H
#include "utils/HttpUtils.h"
#include "service/BorrowService.h"

class UserController {
    BorrowService *borrowService;

public:
    UserController(BorrowService *borrowService);

    ~UserController() = default;

    //GET /api/reader/stats - 获取用户信息
    void handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const;
};


#endif //LIBRARYMANAGEMENTSYSTEM_USERCONTROLLER_H
