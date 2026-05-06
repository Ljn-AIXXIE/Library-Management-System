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
    if (string err; !userService->superAdminListManagers(operatorUserId, rows, err)) {
        res = HttpUtils::createErrorResponse(err, 403);
        return;
    }

    nlohmann::json arr = nlohmann::json::array();
    for (const auto &[userId, name, role]: rows) {
        arr.push_back({
            {"userId", userId},
            {"name", name},
            {"role", role}
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
    if (string errorMsg; !
        HttpUtils::validateRequiredFields(requestData, {"operatorUserId", "targetUserId"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }
    const string operatorUserId = requestData["operatorUserId"].get<string>();
    const string targetUserId = requestData["targetUserId"].get<string>();

    if (string err; !userService->superAdminPromoteStudentToAdmin(operatorUserId, targetUserId, err)) {
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
    if (string errorMsg; !
        HttpUtils::validateRequiredFields(requestData, {"operatorUserId", "targetUserId"}, errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }
    const string operatorUserId = requestData["operatorUserId"].get<string>();
    const string targetUserId = requestData["targetUserId"].get<string>();

    if (string err; !userService->superAdminDemoteAdminToStudent(operatorUserId, targetUserId, err)) {
        res = HttpUtils::createErrorResponse(err, 400);
        return;
    }
    const nlohmann::json responseData = {
        {"success", true},
        {"message", "已撤销该用户的管理员身份"}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

void SuperAdminController::handleAddAdmin(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/super-admin/managers/add 添加管理员");

    const nlohmann::json requestData = HttpUtils::parseRequestBody(req);
    if (string errorMsg; !
        HttpUtils::validateRequiredFields(requestData, {"operatorUserId", "targetUserId", "targetUserName"},
                                          errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }
    const string operatorUserId = requestData["operatorUserId"].get<string>();
    const string targetUserId = requestData["targetUserId"].get<string>();
    const string targetUserName = requestData["targetUserName"].get<string>();

    if (string err; !userService->superAdminAddAdmin(operatorUserId, targetUserId, targetUserName, err)) {
        res = HttpUtils::createErrorResponse(err, 400);
        return;
    }
    const nlohmann::json responseData = {
        {"success", true},
        {"message", "已成功添加管理员"}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}

void SuperAdminController::handleDeleteAdmin(const httplib::Request &req, httplib::Response &res) const {
    Logger::getInstance().logAccess("POST /api/super-admin/managers/delete 删除管理员");

    const nlohmann::json requestData = HttpUtils::parseRequestBody(req);
    if (string errorMsg; !
        HttpUtils::validateRequiredFields(requestData, {"operatorUserId", "targetUserId"},
                                          errorMsg)) {
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }
    const string operatorUserId = requestData["operatorUserId"].get<string>();
    const string targetUserId = requestData["targetUserId"].get<string>();

    if (string err; !userService->superAdminDeleteAdmin(operatorUserId, targetUserId, err)) {
        res = HttpUtils::createErrorResponse(err, 400);
        return;
    }
    const nlohmann::json responseData = {
        {"success", true},
        {"message", "已成功删除管理员"}
    };
    res = HttpUtils::createSuccessResponse(responseData, 200);
}
