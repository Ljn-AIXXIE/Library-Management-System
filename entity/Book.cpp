#include "Book.h"
#include <iostream>
using std::string;

Book::Book(const string &id, const string &title, const string &author, const string &category) {
    this->id = id;
    this->title = title;
    this->author = author;
    this->category = category;
    this->publisher = "";
    this->publishDate = "";
    this->price = "";
    this->pages = "";
    this->description = "";
}

Book::Book(const string &id, const string &title, const string &author, const string &category, const string &publisher,
           const string &publishDate, const string &price, const string &pages, const string &description) {
    this->id = id;
    this->title = title;
    this->author = author;
    this->category = category;
    this->publisher = publisher;
    this->publishDate = publishDate;
    this->price = price;
    this->pages = pages;
    this->description = description;
}

Book::Book() {
    this->id = "";
    this->title = "";
    this->author = "";
    this->category = "";
    this->publisher = "";
}

Book::~Book() = default;

//获取图书id
string Book::getId() const {
    return id;
}

//获取图书名称
string Book::getTitle() const {
    return title;
}

//获取图书作者
string Book::getAuthor() const {
    return author;
}

//获取图书分类
string Book::getCategory() const {
    return category;
}

//获取图书出版社
string Book::getPublisher() const {
    return publisher;
}

//获取图书出版日期
string Book::getPublishDate() const {
    return publishDate;
}

//获取图书价格
string Book::getPrice() const {
    return price;
}

//获取图书页数
string Book::getPages() const {
    return pages;
}

//获取图书简介
string Book::getDescription() const {
    return description;
}

//设置图书名称
void Book::setBookTitle(const string &title) {
    this->title = title;
}

//设置图书作者
void Book::setAuthor(const string &author) {
    this->author = author;
}

//设置图书分类
void Book::setCategory(const string &category) {
    this->category = category;
}

//设置图书出版社
void Book::setPublisher(const string &publisher) {
    this->publisher = publisher;
}

void Book::setBookId(const string &id) {
    this->id = id;
}

void Book::setPublishDate(const string &publishDate) {
    this->publishDate = publishDate;
}

void Book::setPrice(const string &price) {
    this->price = price;
}

void Book::setPages(const string &pages) {
    this->pages = pages;
}

void Book::setDescription(const string &description) {
    this->description = description;
}
