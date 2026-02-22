#ifndef LIBRARY_MANAGEMENT_SYSTEM_ADMINBLACKLISTCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_ADMINBLACKLISTCONTROLLER_H

#include"services/BlackListService.h"
#include"utils/HttpUtils.h"

class AdminBlackListController {
    BlackListService *blackListService;
public:
    explicit AdminBlackListController(BlackListService *blackListService) : blackListService(blackListService) {}

    //POST /api/admin/readers/freeze - 添加黑名单用户
    void handleFreezeUser(const httplib::Request &req, httplib::Response &res) const;

    //POST /api/admin/readers/unfreeze - 移除黑名单用户
    void handleUnfreezeUser(const httplib::Request &req, httplib::Response &res) const;

    // //GET /api/admin/blacklist - 获取黑名单用户列表
    // void handleGetBlackList(httplib::Response& res) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_ADMINBLACKLISTCONTROLLER_H
