#ifndef LIBRARY_MANAGEMENT_SYSTEM_USER_H
#define LIBRARY_MANAGEMENT_SYSTEM_USER_H

#include <string>
#include <utility>

#include "utils/PasswordUtils.h"

class User {
    std::string id;
    std::string name;
    std::string password;
    std::string type;
    std::string status = "normal";
    int borrowedBookCount = 0;

public:
    User() = default;
    User(std::string id, std::string name, const std::string &password,
         std::string type = "reader", const int borrowedBookCount = 0)
        : id(std::move(id)), name(std::move(name)), password(PasswordUtils::encryptPassword(password)),
          type(std::move(type)), borrowedBookCount(borrowedBookCount) {}

    [[nodiscard]] const std::string& getId() const { return id; }
    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] const std::string& getPassword() const { return password; }
    [[nodiscard]] const std::string& getType() const { return type; }
    [[nodiscard]] const std::string& getStatus() const { return status; }
    [[nodiscard]] int getBorrowedBookCount() const { return borrowedBookCount; }

    void setId(const std::string &newId) { id = newId; }
    void setName(const std::string &newName) { name = newName; }
    void setPassword(const std::string &newPassword) {
        password = PasswordUtils::encryptPassword(newPassword);
    }
    void setType(const std::string &newType) { type = newType; }
    void setStatus(const std::string &newStatus) { status = newStatus; }
    void setBorrowedBookCount(const int newCount) { borrowedBookCount = newCount; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_USER_H