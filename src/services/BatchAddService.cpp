#include "services/BatchAddService.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "utils/SqlToDatabaseWriter.h"

void BatchAddService::addBooksFromTxt(const char *data, size_t length,
                                      std::vector<std::string> &errorList) const {
    std::string chunk(data, length);
    TxtToDatabaseWriter::writeBooksToDatabase(chunk, bookDAO, inventoryService, errorList);
}

void BatchAddService::addUsersFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const {
    std::string chunk(data, length);
    TxtToDatabaseWriter::writeUsersToDatabase(chunk, userDAO, errorList);
}

void BatchAddService::addBookCopiesFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const {
    std::string chunk(data, length);
    TxtToDatabaseWriter::writeBookCopiesToDatabase(chunk, bookDAO, inventoryService, errorList);
}

void BatchAddService::addRecordsFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const {
    std::string chunk(data, length);
    TxtToDatabaseWriter::writeRecordsToDatabase(data, recordDAO, errorList);
}

void BatchAddService::addBlackListFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const {
    std::string chunk(data, length);
    TxtToDatabaseWriter::wireBlackListToDatabase(chunk, blackListDAO, errorList);
}

void BatchAddService::addFromSql(const char *data, size_t length, std::vector<std::string> &errorList) const {
    std::string chunk(data, length);
    SqlToDatabaseWriter::writeSqlToDatabase(chunk, databaseOperator);
}

bool BatchAddService::validateSql(const char *data, size_t length) const {
    std::string sql(data, length);

    if (sql.empty()) {
        std::cout << "SQL文件为空" << std::endl; //后续写入日志
        return false;
    }

    std::vector<std::string> forbidden = {
        "DROP TABLE",
        "DROP DATABASE",
        "ALTER TABLE",
        "DELETE FROM"
    };

    std::string upperSql = sql;
    std::ranges::transform(upperSql, upperSql.begin(), ::toupper);

    for (const auto &keyword: forbidden) {
        if (upperSql.find(keyword) != std::string::npos) {
            std::cout << "SQL文件包含禁用关键字: " << keyword << std::endl; //后续写入日志
            return false;
        }
    }

    if (!databaseOperator->beginTransaction()) return false;

    std::stringstream ss(sql);
    std::string line;
    while (getline(ss, line, ';')) {
        if (line.find_first_not_of("\n\r\t") == std::string::npos) continue;
        if (!databaseOperator->execute(line)) {
            databaseOperator->rollback();
            return false;
        }
    }

    databaseOperator->rollback();
    return true;
}
