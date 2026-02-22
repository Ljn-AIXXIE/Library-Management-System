#include "database/dao/BlackListDAO.h"

#include "common/Logger.h"

bool BlackListDAO::addBlackList(const std::string &userId) const {
    const std::string sql =
            "INSERT INTO black_list (user_id) "
            "VALUES (?);";

    sqlite3_stmt *stmt = nullptr;
    if (!blackListDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BlackListDAO::addBlackList准备SQL失败:" + blackListDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        blackListDatabase->setLastError(sqlite3_errmsg(blackListDatabase->getDB()));
        Logger::getInstance().logError("BlackListDAO::addBlackList执行SQL失败:" + blackListDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool BlackListDAO::removeBlackList(const std::string &userId) const {
    const std::string sql = "DELETE FROM black_list WHERE user_id = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (!blackListDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BlackListDAO::removeBlackList准备SQL失败:" + blackListDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        blackListDatabase->setLastError(sqlite3_errmsg(blackListDatabase->getDB()));
        Logger::getInstance().logError("BlackListDAO::removeBlackList执行SQL失败:" + blackListDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool BlackListDAO::isBlackListed(const std::string &userId) const {
    const std::string sql = "SELECT COUNT(*) FROM black_list WHERE user_id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!blackListDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BlackListDAO::isBlackListed准备SQL失败:" + blackListDatabase->getLastError());
        return false;
    }

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        Logger::getInstance().logError("BlackListDAO::isBlackListed执行SQL失败:" + blackListDatabase->getLastError());
    }
    sqlite3_finalize(stmt);
    return count > 0;
}
