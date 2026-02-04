#include "AdminUserController.h"

AdminUserController::AdminUserController(UserService *userService,BlackListService *blackListService): userService(userService), blackListService(blackListService) {}

AdminUserController::~AdminUserController()= default;

void AdminUserController::handleGetAllUsers(httplib::Response &res) const {
    cout << "[AdminUserController] 获取所有用户列表" << endl;

    vector<User> users = userService->getAllUsers();

    //通过查询黑名单来设定读者状态
    for (auto& user : users) {
        if (blackListService->isBlackListed(user.getId())) {
            user.setStatus("frozen");
        }
    }

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

//GET /api/admin/readers/detail?userId=<userId> - 获取读者详细信息
void AdminUserController::handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const {
    if (!req.has_param("userId")) {
        cout << "获取读者详细信息失败: 缺少userId参数" << endl;
        res = HttpUtils::createErrorResponse("缺少userId参数", 400);
        return;
    }

    const string userId = req.get_param_value("userId");
    User user;
    if (!userService->getUserById(userId, user)) {
        cout << "获取读者详细信息失败: 用户不存在" << endl;
        res = HttpUtils::createErrorResponse("用户不存在", 404);
        return;
    }
    json responseData = {
        {"success", true},
        {"data", {
            {"userId", user.getId()},
            {"name", user.getName()},
            {"status", user.getStatus()},
            {"borrowedBookCount", user.getBorrowedBookCount()},
            {"totalBorrowCount", user.getTotalBorrowCount()},
            {"dueSoonCount", user.getDueSoonCount()},
            {"overdueCount", user.getOverdueCount()}
        }}
    };

    res = HttpUtils::createSuccessResponse(responseData,200);
    cout << "[AdminUserController] 获取读者详细信息成功" << endl;
}