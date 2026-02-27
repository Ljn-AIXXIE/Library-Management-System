#include "controllers/admin/AdminBookExceptionController.h"
#include "common/Logger.h"
#include "entities/BookExceptionReport.h"

//POST /api/exception/submit - 提交异常
void AdminBookExceptionController::handleAddException(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/exception/submit 提交异常");
    nlohmann::json requestData = HttpUtils::parseRequestBody(req);

    std::string copyId = requestData["copyId"];
    std::string exceptionType = requestData["exceptionType"];
    std::string exceptionDescription = requestData["note"];
    std::string reporterId = requestData["adminId"];

    if (!inventoryService->isCopyBookIdExist(copyId)) {
        res = HttpUtils::createErrorResponse("副本ID不存在，请重新检查", 404);
        return;
    }

    BookExceptionReport report(copyId, reporterId, exceptionType, exceptionDescription);
    if (bookExceptionReportService->addBookExceptionReport(report) && inventoryService->updateBookCopyStatus(
            copyId, exceptionType)) {
        const nlohmann::json responseData = {
            {"success", true},
            {"message", "异常报告提交成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        res = HttpUtils::createErrorResponse("异常报告提交失败", 500);
    }
}
