#include "database/DatabaseOperator.h"

#include "common/Logger.h"

DatabaseOperator::~DatabaseOperator() {
    close();
}

bool DatabaseOperator::open(const std::string &daPath) {
    if (const int result = sqlite3_open(daPath.c_str(), &database); result != SQLITE_OK) {
        lastError = sqlite3_errmsg(database);
        Logger::getInstance().logError("DatabaseOperator::open数据库连接失败:" + lastError);
        close();
        return false;
    }
    return true;
}

void DatabaseOperator::close() {
    if (database) {
        sqlite3_close(database);
        database = nullptr;
    }
}

bool DatabaseOperator::execute(const std::string &sql) {
    char *errorMessage = nullptr;
    if (const int result = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, &errorMessage); result != SQLITE_OK) {
        lastError = errorMessage ? errorMessage : sqlite3_errmsg(database);
        sqlite3_free(errorMessage);

        Logger::getInstance().logError("DatabaseOperator::execute执行SQL失败:" + lastError);

        return false;
    }

    return true;
}

bool DatabaseOperator::query(const std::string &sql, std::vector<std::vector<std::string> > &result) {
    sqlite3_stmt *statement = nullptr;
    if (!prepare(sql, &statement)) {
        Logger::getInstance().logError("DatabaseOperator::query准备SQL失败:" + lastError);

        return false;
    }
    const int columnCount = sqlite3_column_count(statement);
    while (sqlite3_step(statement) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < columnCount; i++) {
            const unsigned char *text = sqlite3_column_text(statement, i);
            row.push_back(text ? reinterpret_cast<const char *>(text) : "");
        }
        result.push_back(row);
    }
    sqlite3_finalize(statement);

    return true;
}

bool DatabaseOperator::prepare(const std::string &sql, sqlite3_stmt **statement) {
    if (const int result = sqlite3_prepare_v2(database, sql.c_str(), -1, statement, nullptr); result != SQLITE_OK) {
        if (*statement) {
            sqlite3_finalize(*statement);
            *statement = nullptr;
        }
        lastError = sqlite3_errmsg(database);

        Logger::getInstance().logError("DatabaseOperator::prepare准备SQL失败:" + lastError);

        return false;
    }

    return true;
}

bool DatabaseOperator::beginTransaction() {
    return execute("BEGIN TRANSACTION;");
}

bool DatabaseOperator::commit() {
    return execute("COMMIT;");
}

bool DatabaseOperator::rollback() {
    return execute("ROLLBACK;");
}
