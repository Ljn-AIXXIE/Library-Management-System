#include "AdminUserController.h"

AdminUserController::AdminUserController(UserService *userService): userService(userService) {}

AdminUserController::~AdminUserController()= default;

void AdminUserController::handleGetAllUsers(httplib::Response &res) const {
    cout << "[AdminUserController] 获取所有用户列表" << endl;

    vector<User> users = userService->getAllUsers();

    //通过查询黑名单来设定读者状态

    json usersArray = json::array();
    for (const auto& user : users) {
        usersArray.push_back({
            {"userId", user.getId()},
            {"name", user.getName()},
            {"borrowedBookCount", user.getBorrowedBookCount()},
            {"status", user.getStatus()}
        });
    }

    json responseData = {
        {"success", true},
        {"data", usersArray}
    };

    cout << "[AdminUserController] 返回 " << users.size() << " 个用户" << endl;
    res = HttpUtils::createSuccessResponse(responseData, 200);
}
