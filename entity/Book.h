#ifndef AVL_BOOKSYSTEM_BOOK_H
#define AVL_BOOKSYSTEM_BOOK_H
#include <string>
#include <chrono>
using namespace std;

class Book {
    string id; //图书的id
    string title; //图书的名称
    string author; //图书的作者
    string category; //图书的分类
    string publisher; //图书的出版社(可选)
    string publishDate; //图书的出版日期(可选)
    string price; //图书的价格(可选)
    string pages; //图书的页数(可选)
    string description; //图书的简介(可选)

public:
    Book(const string &id, const string &title, const string &author, const string &category);

    Book(const string &id, const string &title, const string &author, const string &category, const string &publisher,
         const string &publishDate, const string &price, const string &pages, const string &description);

    Book();

    ~Book();

    [[nodiscard]] string getId() const; //获取图书id
    [[nodiscard]] string getTitle() const; //获取图书名称
    [[nodiscard]] string getAuthor() const; //获取图书作者
    [[nodiscard]] string getCategory() const; //获取图书分类
    [[nodiscard]] string getPublisher() const; //获取图书出版社
    [[nodiscard]] string getPublishDate() const; //获取图书出版日期
    [[nodiscard]] string getPrice() const; //获取图书价格
    [[nodiscard]] string getPages() const; //获取图书页数
    [[nodiscard]] string getDescription() const; //获取图书简介

    void setBookTitle(const string &title); //设置图书名称
    void setAuthor(const string &author); //设置图书作者
    void setCategory(const string &category); //设置图书分类
    void setPublisher(const string &publisher); //设置图书出版社
    void setBookId(const string &id);

    void setPublishDate(const string &publishDate); //设置图书出版日期
    void setPrice(const string &price); //设置图书价格
    void setPages(const string &pages); //设置图书页数
    void setDescription(const string &description); //设置图书简介

    void printBookInfo(bool isAdmin) const; //打印图书信息
};

#endif //AVL_BOOKSYSTEM_BOOK_H
