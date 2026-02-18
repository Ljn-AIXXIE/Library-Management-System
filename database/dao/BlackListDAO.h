#ifndef LIBRARYMANAGEMENTSYSTEM_BLACKLISTDAO_H
#define LIBRARYMANAGEMENTSYSTEM_BLACKLISTDAO_H
#include "../DatabaseOperator.h"
using std::string;


class BlackListDAO {
    DatabaseOperator *blackListDatabase;

public:
    explicit BlackListDAO(DatabaseOperator *blackListDatabase);

    ~BlackListDAO();

    //核心操作
    [[nodiscard]] bool addBlackList(const string &userId) const; //添加黑名单用户
    [[nodiscard]] bool removeBlackList(const string &userId) const; //移除黑名单用户

    //查询操作
    [[nodiscard]] bool isBlackListed(const string &userId) const; //查询用户是否在黑名单中
};


#endif //LIBRARYMANAGEMENTSYSTEM_BLACKLISTDAO_H
