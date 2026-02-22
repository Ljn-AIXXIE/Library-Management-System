#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOK_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOK_H

#include <string>
#include <utility>

class Book {
    std::string id;             // 图书ID
    std::string title;          // 图书名称
    std::string author;         // 作者
    std::string category;       // 分类
    std::string publisher;      // 出版社（可选）
    std::string publishDate;    // 出版日期（可选）
    std::string price;          // 价格（可选）
    std::string pages;          // 页数（可选）
    std::string description;    // 简介（可选）

public:
    Book() = default;
    Book(std::string id, std::string title, std::string author, std::string category)
        : id(std::move(id)), title(std::move(title)), author(std::move(author)), category(std::move(category)) {}
    Book(std::string id, std::string title, std::string author, std::string category,
         std::string publisher, std::string publishDate, std::string price,
         std::string pages, std::string description)
        : id(std::move(id)), title(std::move(title)), author(std::move(author)), category(std::move(category)), publisher(std::move(publisher)),
          publishDate(std::move(publishDate)), price(std::move(price)), pages(std::move(pages)), description(std::move(description)) {}

    [[nodiscard]] const std::string& getId() const { return id; }
    [[nodiscard]] const std::string& getTitle() const { return title; }
    [[nodiscard]] const std::string& getAuthor() const { return author; }
    [[nodiscard]] const std::string& getCategory() const { return category; }
    [[nodiscard]] const std::string& getPublisher() const { return publisher; }
    [[nodiscard]] const std::string& getPublishDate() const { return publishDate; }
    [[nodiscard]] const std::string& getPrice() const { return price; }
    [[nodiscard]] const std::string& getPages() const { return pages; }
    [[nodiscard]] const std::string& getDescription() const { return description; }

    void setBookId(const std::string &newId) { id = newId; }
    void setBookTitle(const std::string &newTitle) { title = newTitle; }
    void setAuthor(const std::string &newAuthor) { author = newAuthor; }
    void setCategory(const std::string &newCategory) { category = newCategory; }
    void setPublisher(const std::string &newPublisher) { publisher = newPublisher; }
    void setPublishDate(const std::string &newPublishDate) { publishDate = newPublishDate; }
    void setPrice(const std::string &newPrice) { price = newPrice; }
    void setPages(const std::string &newPages) { pages = newPages; }
    void setDescription(const std::string &newDescription) { description = newDescription; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOK_H