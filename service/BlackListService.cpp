#include "BlackListService.h"
#include "common/Logger.h"

BlackListService::BlackListService(BlackListDAO *db) : db(db) {
}

BlackListService::~BlackListService() = default;

bool BlackListService::addBlackList(const string &userId) const {
    if (db->addBlackList(userId)) {
        Logger::getInstance().logBusiness("用户" + userId + "加入黑名单成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "加入黑名单失败");
    return false;
}

bool BlackListService::removeBlackList(const string &userId) const {
    if (db->removeBlackList(userId)) {
        Logger::getInstance().logBusiness("用户" + userId + "移出黑名单成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "移出黑名单失败");
    return false;
}

bool BlackListService::isBlackListed(const string &userId) const {
    return db->isBlackListed(userId);
}
