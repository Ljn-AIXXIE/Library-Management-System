#include "UserController.h"

UserController::UserController(BorrowService *borrowService) : borrowService(borrowService) {
}

void UserController::handleGetUserDetail(const httplib::Request &req, httplib::Response &res) const {
    if (!req.has_param("userId")) {
        cout << "获取用户详细信息失败: 缺少userId参数" << endl;
        res = HttpUtils::createErrorResponse("缺少userId参数", 400);
        return;
    }

    const string userId = req.get_param_value("userId");

    int currentBorrowCount = borrowService->getCurrentBorrowCount(userId);
    int total = borrowService->getTotalBorrowCount(userId);
    int dueSoon = borrowService->getDueSoonCount(userId);
    int overdue = borrowService->getOverdueCount(userId);

    json responseData = {
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
    cout << "[UserController] 返回用户详细信息" << endl;
}
