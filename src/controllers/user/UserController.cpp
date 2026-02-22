#include "controllers/user/UserController.h"

#include "common/Logger.h"

using std::string;

void UserController::handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/reader/stats 获取用户信息");

    if (!req.has_param("userId")) {
        res = HttpUtils::createErrorResponse("缺少userId参数", 400);
        return;
    }

    const string userId = req.get_param_value("userId");

    int currentBorrowCount = borrowService->getCurrentBorrowCount(userId);
    int total = borrowService->getTotalBorrowCount(userId);
    int dueSoon = borrowService->getDueSoonCount(userId);
    int overdue = borrowService->getOverdueCount(userId);

    const nlohmann::json responseData = {
        {"success", true},
        {
            "data", {
                {"currentBorrow", currentBorrowCount},
                {"totalBorrow", total},
                {"dueSoon", dueSoon},
                {"overdue", overdue}
            }
        }
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}
