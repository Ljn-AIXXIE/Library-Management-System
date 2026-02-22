#ifndef LIBRARY_MANAGEMENT_SYSTEM_BLACKLISTDAO_H
#define LIBRARY_MANAGEMENT_SYSTEM_BLACKLISTDAO_H

#include "database/DatabaseOperator.h"

class BlackListDAO {
    DatabaseOperator *blackListDatabase;
public:
    explicit BlackListDAO(DatabaseOperator *blackListDatabase) : blackListDatabase(blackListDatabase) {}

    //核心操作
    [[nodiscard]] bool addBlackList(const std::string &userId) const; //添加黑名单用户
    [[nodiscard]] bool removeBlackList(const std::string &userId) const; //移除黑名单用户

    //查询操作
    [[nodiscard]] bool isBlackListed(const std::string &userId) const; //查询用户是否在黑名单中
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BLACKLISTDAO_H
