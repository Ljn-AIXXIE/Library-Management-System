#include "AdminBlackListController.h"

AdminBlackListController::AdminBlackListController(BlackListService *blackListService) : blackListService(blackListService) {}

AdminBlackListController::~AdminBlackListController()=default;

//POST /api/admin/readers/freeze - 添加黑名单用户
void AdminBlackListController::handleFreezeUser(const httplib::Request &req, httplib::Response &res) const {
    json requestData = HttpUtils::parseRequestBody(req);

    //验证必填字段
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData,{"userId"},errorMsg)) {
        cout << "添加黑名单用户失败: 缺少必填字段 - " << errorMsg << endl;
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    string userId = requestData["userId"];
    if (blackListService->isBlackListed(userId)) {
        cout << "添加黑名单用户失败: 用户已存在黑名单中" << endl;
        res = HttpUtils::createErrorResponse("用户已存在黑名单中", 400);
        return;
    }
    if (blackListService->addBlackList(userId)) {
        json responseData = {
            {"success", true},
            {"message", "黑名单用户添加成功"}
        };
        cout << "黑名单用户添加成功！" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 201);
    } else {
        cout << "添加黑名单用户失败" << endl;
        res = HttpUtils::createErrorResponse("黑名单用户添加失败", 500);
    }
}

//POST /api/admin/readers/unfreeze - 移除黑名单用户
void AdminBlackListController::handleUnfreezeUser(const httplib::Request &req, httplib::Response &res) const {
    json requestData = HttpUtils::parseRequestBody(req);

    //验证必填字段
    string errorMsg;
    if (!HttpUtils::validateRequiredFields(requestData,{"userId"},errorMsg)) {
        cout << "移除黑名单用户失败: 缺少必填字段 - " << errorMsg << endl;
        res = HttpUtils::createErrorResponse(errorMsg, 400);
        return;
    }

    string userId = requestData["userId"];
    if (!blackListService->isBlackListed(userId)) {
        cout << "移除黑名单用户失败: 用户不存在黑名单中" << endl;
        res = HttpUtils::createErrorResponse("用户不存在黑名单中", 400);
        return;
    }
    if (blackListService->removeBlackList(userId)) {
        json responseData = {
            {"success", true},
            {"message", "黑名单用户移除成功"}
        };
        cout << "黑名单用户移除成功！" << endl;
        res = HttpUtils::createSuccessResponse(responseData, 200);
    } else {
        cout<< "移除黑名单用户失败" << endl;
        res = HttpUtils::createErrorResponse("黑名单用户移除失败", 500);
    }
}