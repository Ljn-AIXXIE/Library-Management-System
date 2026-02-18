#ifndef AVL_BOOKSYSTEM_USER_H
#define AVL_BOOKSYSTEM_USER_H
#include <string>
#include <vector>
using std::string;

class User {
    string id; //用户id
    string name; //用户名
    string password; //用户密码
    string type; //用户类型
    string status = "normal";
    int borrowedBookCount; //用户已借阅数量

public:
    User(const string &id, const string &name, string password, const string &type = "reader",
         const int borrowedBookCount = 0);

    User();

    ~User();

    [[nodiscard]] string getId() const; //获取用户id
    [[nodiscard]] string getName() const; //获取用户名
    [[nodiscard]] string getPassword() const; //获取用户密码
    [[nodiscard]] string getType() const; //获取用户类型
    [[nodiscard]] string getStatus() const; //获取用户状态
    [[nodiscard]] int getBorrowedBookCount() const; //获取用户已借阅数量

    void setId(string id);

    void setName(string name);

    void setPassword(string password); //设置用户密码
    void setType(string type); //设置用户类型
    void setStatus(string status);

    void setBorrowedBookCount(int borrowedBookCount);
};

#endif //AVL_BOOKSYSTEM_USER_H
