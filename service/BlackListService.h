#ifndef LIBRARYMANAGEMENTSYSTEM_BLACKLISTSERVICE_H
#define LIBRARYMANAGEMENTSYSTEM_BLACKLISTSERVICE_H
#include "../database/dao/BlackListDAO.h"

class BlackListService {
    BlackListDAO* db;
public:
    BlackListService(BlackListDAO* db);
    ~BlackListService();

    //核心操作
    [[nodiscard]] bool addBlackList(const string& userId) const;//添加黑名单用户
    [[nodiscard]] bool removeBlackList(const string& userId) const;//移除黑名单用户

    //查询操作
    [[nodiscard]] bool isBlackListed(const string& userId) const;//查询用户是否在黑名单中
};


#endif //LIBRARYMANAGEMENTSYSTEM_BLACKLISTSERVICE_H