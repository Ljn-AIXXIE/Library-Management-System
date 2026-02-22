#include "controllers/admin/AdminUserController.h"

#include "utils/TimeUtils.h"
#include "common/Logger.h"

using std::string;
using std::vector;

void AdminUserController::handleGetAllUsers(httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/readers 获取所有读者");
    vector<User> users = userService->getAllUsers();

    //通过查询黑名单来设定读者状态
    for (auto &user: users) {
        if (blackListService->isBlackListed(user.getId())) {
            user.setStatus("frozen");
        }
    }

    nlohmann::json usersArray = nlohmann::json::array();
    for (const auto &user: users) {
        usersArray.push_back({
            {"userId", user.getId()},
            {"name", user.getName()},
            {"borrowedBookCount", user.getBorrowedBookCount()},
            {"status", user.getStatus()}
        });
    }

    const nlohmann::json responseData = {
        {"success", true},
        {"data", usersArray}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

//GET /api/admin/readers/detail?userId=<userId> - 获取读者详细信息
void AdminUserController::handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/readers/detail 获取读者详细信息");

    if (!req.has_param("userId")) {
        res = HttpUtils::createErrorResponse("缺少userId参数", 400);
        return;
    }

    const string userId = req.get_param_value("userId");
    User user;
    if (!userService->getUserById(userId, user)) {
        res = HttpUtils::createErrorResponse("用户不存在", 404);
        return;
    }

    int totalBorrowCount = borrowService->getTotalBorrowCount(userId);
    int dueSoonCount = borrowService->getDueSoonCount(userId);
    int overdueCount = borrowService->getOverdueCount(userId);
    vector<Record> records = borrowService->getUserBorrowingRecords(userId);
    nlohmann::json borrows = nlohmann::json::array();
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

    const nlohmann::json responseData = {
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
}

//GET /api/admin/readers/search?userId=<userId> - 搜索读者
void AdminUserController::handleSearchUsers(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/readers/search 搜索读者");

    if (!req.has_param("userId")) {
        res = HttpUtils::createErrorResponse("缺少userId参数", 400);
        return;
    }

    string userId = req.get_param_value("userId");
    User user;
    if (!userService->getUserById(userId, user)) {
        res = HttpUtils::createErrorResponse("用户不存在", 404);
        return;
    }

    nlohmann::json usersArray = nlohmann::json::array();
    usersArray.push_back({
        {"userId", user.getId()},
        {"name", user.getName()},
        {"borrowedBookCount", user.getBorrowedBookCount()},
        {"status", user.getStatus()}
    });
    const nlohmann::json responseData = {
        {"success", true},
        {"data", usersArray}
    };

    res = HttpUtils::createSuccessResponse(responseData, 200);
}
