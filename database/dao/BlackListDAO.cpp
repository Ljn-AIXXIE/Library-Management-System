#include "BlackListDAO.h"

BlackListDAO::BlackListDAO(DatabaseOperator *blackListDatabase) : blackListDatabase(blackListDatabase) {
}

BlackListDAO::~BlackListDAO() = default;

bool BlackListDAO::addBlackList(const string &userId) const {
    const string sql =
            "INSERT INTO black_list (user_id) "
            "VALUES (?);";

    sqlite3_stmt *stmt = nullptr;
    if (!blackListDatabase->prepare(sql, &stmt)) return false;

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool BlackListDAO::removeBlackList(const string &userId) const {
    const string sql = "DELETE FROM black_list WHERE user_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!blackListDatabase->prepare(sql, &stmt)) return false;

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool BlackListDAO::isBlackListed(const string &userId) const {
    const string sql = "SELECT COUNT(*) FROM black_list WHERE user_id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!blackListDatabase->prepare(sql, &stmt)) return false;

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count > 0;
}
