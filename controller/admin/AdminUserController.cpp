#include "AdminUserController.h"
#include "../utils/TimeUtils.h"

AdminUserController::AdminUserController(UserService *userService,
                                         BlackListService *blackListService,
                                         BorrowService *borrowService,
                                         InventoryService *inventoryService) : userService(userService),
                                                                               blackListService(blackListService),
                                                                               borrowService(borrowService),
                                                                               inventoryService(inventoryService) {
}

AdminUserController::~AdminUserController() = default;

void AdminUserController::handleGetAllUsers(httplib::Response &res) const {
    cout << "[AdminUserController] 获取所有用户列表" << endl;

    vector<User> users = userService->getAllUsers();

    //通过查询黑名单来设定读者状态
    for (auto &user: users) {
        if (blackListService->isBlackListed(user.getId())) {
            user.setStatus("frozen");
        }
    }

    json usersArray = json::array();
    for (const auto &user: users) {
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

    int totalBorrowCount = borrowService->getTotalBorrowCount(userId);
    int dueSoonCount = borrowService->getDueSoonCount(userId);
    int overdueCount = borrowService->getOverdueCount(userId);
    vector<Record> records = borrowService->getUserBorrowingRecords(userId);
    json borrows = json::array();
    for (const auto &record: records) {
        string bookId = record.getCopyId().substr(0, record.getCopyId().find_last_of('_'));
        string bookTitle = inventoryService->getBookTitleById(bookId);
        string dueTime = TimeUtils::formatTime(record.getDueTime());
        string borrowTime = TimeUtils::formatTime(record.getBorrowTime());

        borrows.push_back({
            {"dueDate", dueTime},
            {"bookTitle", bookTitle},
            {"borrowDate", borrowTime}
        });
    }

    json responseData = {
        {"success", true},
        {
            "data", {
                {"userId", user.getId()},
                {"name", user.getName()},
                {"status", user.getStatus()},
                {"borrowedBookCount", user.getBorrowedBookCount()},
                {"totalBorrowCount", totalBorrowCount},
                {"dueSoonCount", dueSoonCount},
                {"overdueCount", overdueCount},
                {"borrows", borrows}
            }
        }
    };

    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[AdminUserController] 获取读者详细信息成功" << endl;
}

//GET /api/admin/readers/search?userId=<userId> - 搜索读者
void AdminUserController::handleSearchUsers(const httplib::Request &req, httplib::Response &res) const {
    cout << "[AdminUserController] 搜索读者" << endl;
    if (!req.has_param("userId")) {
        cout << "搜索读者失败: 缺少userId参数" << endl;
        res = HttpUtils::createErrorResponse("缺少userId参数", 400);
        return;
    }

    string userId = req.get_param_value("userId");
    User user;
    if (!userService->getUserById(userId, user)) {
        cout << "搜索读者失败: 用户不存在" << endl;
        res = HttpUtils::createErrorResponse("用户不存在", 404);
        return;
    }

    json usersArray = json::array();
    usersArray.push_back({
        {"userId", user.getId()},
        {"name", user.getName()},
        {"borrowedBookCount", user.getBorrowedBookCount()},
        {"status", user.getStatus()}
    });
    json responseData = {
        {"success", true},
        {"data", usersArray}
    };

    res = HttpUtils::createSuccessResponse(responseData, 200);
    cout << "[AdminUserController] 返回 1 个用户" << endl;
}
