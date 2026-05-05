#include "controllers/admin/SuperAdminController.h"

#include <vector>

#include "common/Logger.h"
#include "utils/HttpUtils.h"

using std::string;

void SuperAdminController::handleListManagers(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("GET /api/super-admin/managers 列出管理员与超级管理员");

    const string operatorUserId = req.get_param_value("operatorUserId");
    if (operatorUserId.empty()) {
        res = HttpUtils::createErrorResponse("缺少参数 operatorUserId", 400);
        return;
    }

    std::vector<UserDAO::AdministratorRecord> rows;
    string err;
    if (!userService->superAdminListManagers(operatorUserId, rows, err)) {
        res = HttpUtils::createErrorResponse(err, 403);
        return;
    }

    nlohmann::json arr = nlohmann::json::array();
    for (const auto &r : rows) {
        arr.push_back({
            {"userId", r.userId},
            {"name", r.name},
            {"role", r.role}
        });
    }
    const nlohmann::json responseData = {
        {"success", true},
        {"data", arr}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

void SuperAdminController::handlePromote(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/super-admin/managers/promote 读者提拔为管理员");

    const nlohmann::json requestData = HttpUtils::parseRequestBody(req);
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"operatorUserId", "targetUserId"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }
    const string operatorUserId = requestData["operatorUserId"].get<string>();
    const string targetUserId = requestData["targetUserId"].get<string>();

    string err;
    if (!userService->superAdminPromoteStudentToAdmin(operatorUserId, targetUserId, err)) {
        res = HttpUtils::createErrorResponse(err, 400);
        return;
    }
    const nlohmann::json responseData = {
        {"success", true},
        {"message", "已将该读者设为管理员"}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

void SuperAdminController::handleDemote(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/super-admin/managers/demote 撤销管理员");

    const nlohmann::json requestData = HttpUtils::parseRequestBody(req);
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData, {"operatorUserId", "targetUserId"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }
    const string operatorUserId = requestData["operatorUserId"].get<string>();
    const string targetUserId = requestData["targetUserId"].get<string>();

    string err;
    if (!userService->superAdminDemoteAdminToStudent(operatorUserId, targetUserId, err)) {
        res = HttpUtils::createErrorResponse(err, 400);
        return;
    }
    const nlohmann::json responseData = {
        {"success", true},
        {"message", "已撤销该用户的管理员身份"}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}
