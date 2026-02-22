#ifndef LIBRARY_MANAGEMENT_SYSTEM_PASSWORDUTILS_H
#define LIBRARY_MANAGEMENT_SYSTEM_PASSWORDUTILS_H

#include <string>

class PasswordUtils {
public:
    static std::string generateSalt();
    static std::string hashPassword(const std::string &password, const std::string &salt); // 计算哈希值
    static std::string encryptPassword(const std::string &password); // 加密密码
    static bool verifyPassword(const std::string &password, const std::string &encryptedPassword);
private:
    static constexpr int SALT_SIZE = 16; // 盐值大小（字节）
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_PASSWORDUTILS_H
