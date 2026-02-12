#ifndef AVL_BOOKSYSTEM_USER_H
#define AVL_BOOKSYSTEM_USER_H
#include <string>
#include <vector>

using namespace std;

class User {
    string id; //用户id
    string name; //用户名
    string password; //用户密码
    string type; //用户类型
    string status = "normal";
    int borrowedBookCount; //用户已借阅数量

public:
    User(string id, string name, string password, string type = "reader", int borrowedBookCount = 0);

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

    // void addBorrowInfo(string bookId);//添加借阅信息
    // void addBorrowInfoWithTime(string bookId, time_t borrowTime, time_t returnTime);//添加借阅信息（带时间参数，用于从文件读取）
    // bool removeBorrowInfo(string bookId);//移除借阅信息，归还成功返回true，失败返回false
    // bool updateBorrowBookId(string oldBookId, string newBookId);//更新借阅信息中的图书id
    // void printBorrowInfo();//打印借阅信息
};

#endif //AVL_BOOKSYSTEM_USER_H
