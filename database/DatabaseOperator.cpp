#include "DatabaseOperator.h"
#include "common/Logger.h"
#include <iostream>

DatabaseOperator::DatabaseOperator() {
    database = nullptr;
}

DatabaseOperator::~DatabaseOperator() {
    close();
}

bool DatabaseOperator::open(const string &daPath) {
    int result = sqlite3_open(daPath.c_str(), &database);
    if (result != SQLITE_OK) {
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

bool DatabaseOperator::execute(const string &sql) {
    char *errorMessage = nullptr;
    int result = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, &errorMessage);
    if (result != SQLITE_OK) {
        lastError = errorMessage ? errorMessage : sqlite3_errmsg(database);
        sqlite3_free(errorMessage);

        Logger::getInstance().logError("DatabaseOperator::execute执行SQL失败:" + lastError);

        return false;
    }

    return true;
}

bool DatabaseOperator::query(const string &sql, vector<vector<string> > &result) {
    sqlite3_stmt *statement = nullptr;
    if (!prepare(sql, &statement)) {
        Logger::getInstance().logError("DatabaseOperator::query准备SQL失败:" + lastError);

        return false;
    }
    int columnCount = sqlite3_column_count(statement);
    while (sqlite3_step(statement) == SQLITE_ROW) {
        vector<string> row;
        for (int i = 0; i < columnCount; i++) {
            const unsigned char *text = sqlite3_column_text(statement, i);
            row.push_back(text ? reinterpret_cast<const char *>(text) : "");
        }
        result.push_back(row);
    }
    sqlite3_finalize(statement);

    return true;
}

bool DatabaseOperator::prepare(const string &sql, sqlite3_stmt **statement) {
    int result = sqlite3_prepare_v2(database, sql.c_str(), -1, statement, nullptr);
    if (result != SQLITE_OK) {
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

string DatabaseOperator::getLastError() const {
    return lastError;
}
