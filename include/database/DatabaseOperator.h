#ifndef LIBRARY_MANAGEMENT_SYSTEM_DATABASEOPERATOR_H
#define LIBRARY_MANAGEMENT_SYSTEM_DATABASEOPERATOR_H

#include <sqlite3.h>
#include <string>
#include <vector>

class DatabaseOperator {
    sqlite3 *database = nullptr;
    std::string lastError;

public:
    DatabaseOperator() = default;

    ~DatabaseOperator();

    //数据库连接
    bool open(const std::string &daPath);

    void close();

    //执行非查询操作
    bool execute(const std::string &sql);

    //执行查询操作
    bool query(const std::string &sql, std::vector<std::vector<std::string> > &result);

    bool prepare(const std::string &sql, sqlite3_stmt **statement);

    bool beginTransaction();

    bool commit();

    bool rollback();

    [[nodiscard]] const std::string &getLastError() const { return lastError; }

    void setLastError(const std::string &error) { lastError = error; }

    sqlite3 *getDB() const { return database; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_DATABASEOPERATOR_H
