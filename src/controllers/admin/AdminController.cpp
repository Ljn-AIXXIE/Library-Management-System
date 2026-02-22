#include "controllers/admin/AdminController.h"

#include "common/Logger.h"
#include "utils/HttpUtils.h"

//GET /api/admin/stats - 获取管理员统计信息
void AdminController::handleGetStats(httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/stats 获取管理员统计信息");

    int totalBooks = inventoryService->getTotalCopyCount();
    int totalReaders = userService->getTotalUserCount();
    int currentBorrows = borrowService->getBorrowedBookCount();
    int overdueBooks = borrowService->getOverDueBookCount();

    const nlohmann::json responseData = {
        {"success", true},
        {
            "data", {
                {"totalBooks", totalBooks},
                {"totalReaders", totalReaders},
                {"currentBorrows", currentBorrows},
                {"overdueBooks", overdueBooks}
            }
        }
    };

    res = HttpUtils::createSuccessResponse(responseData, 200);
}
