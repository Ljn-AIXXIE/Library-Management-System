#ifndef LIBRARY_MANAGEMENT_SYSTEM_BLACKLISTSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_BLACKLISTSERVICE_H

#include "database/dao/BlackListDAO.h"

class BlackListService {
    BlackListDAO *db;
public:
    explicit BlackListService(BlackListDAO *db) : db(db) {}

    //核心操作
    [[nodiscard]] bool addBlackList(const std::string &userId) const; //添加黑名单用户
    [[nodiscard]] bool removeBlackList(const std::string &userId) const; //移除黑名单用户

    //查询操作
    [[nodiscard]] bool isBlackListed(const std::string &userId) const; //查询用户是否在黑名单中
};


#endif //LIBRARY_MANAGEMENT_SYSTEM_BLACKLISTSERVICE_H
