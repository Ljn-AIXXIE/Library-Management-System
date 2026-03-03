#include "controllers/admin/AdminBookExceptionController.h"
#include "common/Logger.h"
#include "entities/BookExceptionReport.h"
#include "utils/TimeUtils.h"

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

    if (bookExceptionReportService->isBookExceptionReportExist(copyId)) {
        res = HttpUtils::createErrorResponse("此记录已经存在，请重新检查", 409);
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

//GET /api/exception/search - 加载异常请求
void AdminBookExceptionController::handleGetAllBookExceptionReport(const httplib::Request &req,
                                                                   httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/exception/handle 加载异常请求");
    std::vector<BookExceptionReport> reports;
    std::string errorMessage;

    if (req.has_param("type") && req.has_param("keyword")) {
        std::string type = req.get_param_value("type");
        std::string keyword = req.get_param_value("keyword");
        std::string unhandled = req.get_param_value("unhandled");
        bool unhandledBool = unhandled == "true";

        if (type == "copyId") {
            reports = bookExceptionReportService->getBookExceptionReportByCopyId(keyword, errorMessage, unhandledBool);
        }
        if (type == "reportId") {
            reports = bookExceptionReportService->getBookExceptionReportByReporterId(
                keyword, errorMessage, unhandledBool);
        }
        if (type == "handlerId") {
            reports = bookExceptionReportService->getBookExceptionReportByHandlerId(keyword, errorMessage);
        }
    } else {
        const std::string unhandled = req.get_param_value("unhandled");
        bool unhandledBool = unhandled == "true";

        reports = bookExceptionReportService->getAllBookExceptionReport(errorMessage, unhandledBool);
    }

    if (errorMessage.empty()) {
        nlohmann::json reportsArray = nlohmann::json::array();
        for (const auto &report: reports) {
            std::string submitTime = TimeUtils::formatTime(report.getSubmitTime());
            std::string handledTime = TimeUtils::formatTime(report.getHandledTime());
            reportsArray.push_back({
                {"copyId", report.getCopyId()},
                {"reporterId", report.getReporterId()},
                {"handlerId", report.getHandlerId()},
                {"errorType", report.getErrorType()},
                {"submitTime", submitTime},
                {"handledTime", handledTime}
            });
        }
        const nlohmann::json responseData = {
            {"success", true},
            {"data", reportsArray}
        };
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        res = HttpUtils::createErrorResponse("异常报告加载失败", 500);
    }
}
