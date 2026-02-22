#include "services/BlackListService.h"

#include "common/Logger.h"

bool BlackListService::addBlackList(const std::string &userId) const {
    if (db->addBlackList(userId)) {
        Logger::getInstance().logBusiness("用户" + userId + "加入黑名单成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "加入黑名单失败");
    return false;
}

bool BlackListService::removeBlackList(const std::string &userId) const {
    if (db->removeBlackList(userId)) {
        Logger::getInstance().logBusiness("用户" + userId + "移出黑名单成功");
        return true;
    }
    Logger::getInstance().logError("用户" + userId + "移出黑名单失败");
    return false;
}

bool BlackListService::isBlackListed(const std::string &userId) const {
    return db->isBlackListed(userId);
}
