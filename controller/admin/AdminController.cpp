#include "AdminController.h"
#include "common/Logger.h"
#include "utils/HttpUtils.h"

AdminController::AdminController(UserService *userService,
                                 InventoryService *inventoryService,
                                 BorrowService *borrowService) : userService(userService),
                                                                 inventoryService(inventoryService),
                                                                 borrowService(borrowService) {
}

//GET /api/admin/stats - 获取管理员统计信息
void AdminController::handleGetStats(httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/admin/stats 获取管理员统计信息");

    int totalBooks = inventoryService->getTotalCopyCount();
    int totalReaders = userService->getTotalUserCount();
    int currentBorrows = borrowService->getBorrowedBookCount();
    int overdueBooks = borrowService->getOverDueBookCount();

    json responseData = {
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
