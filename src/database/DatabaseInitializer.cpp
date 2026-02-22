#include "database/DatabaseInitializer.h"

#include <fstream>

#include "common/Logger.h"

bool DatabaseInitializer::initializeDatabase(DatabaseOperator *db) {
    if (!db) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase数据库操作对象为空");
        return false;
    }

    // 创建表
    if (!createBookTable(db)) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase创建book表失败");
        return false;
    }
    if (!createUserTable(db)) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase创建user表失败");
        return false;
    }
    if (!createBookCopyTable(db)) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase创建book_copy表失败");
        return false;
    }
    if (!createRecordTable(db)) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase创建record表失败");
        return false;
    }
    if (!createIndexes(db)) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase创建索引失败");
        return false;
    }
    if (!createBlackListTable(db)) {
        Logger::getInstance().logError("DatabaseInitializer::initializeDatabase创建black_list表失败");
        return false;
    }

    return true;
}

bool DatabaseInitializer::createBookTable(DatabaseOperator *db) {
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS book (
            id TEXT PRIMARY KEY,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            category TEXT,
            publisher TEXT,
            publish_date TEXT,
            price TEXT,
            pages TEXT,
            description TEXT,
            copy_count INTEGER DEFAULT 0
        );
    )";

    if (!db->execute(sql)) {
        Logger::getInstance().logError("DatabaseInitializer::createBookTable执行SQL失败:" + db->getLastError());
        return false;
    }
    return true;
}

bool DatabaseInitializer::createUserTable(DatabaseOperator *db) {
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS user (
            user_id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            role TEXT NOT NULL,
            password TEXT NOT NULL,
            borrow_count INTEGER DEFAULT 0
        );
    )";

    if (!db->execute(sql)) {
        Logger::getInstance().logError("DatabaseInitializer::createUserTable执行SQL失败:" + db->getLastError());
        return false;
    }
    return true;
}

bool DatabaseInitializer::createBookCopyTable(DatabaseOperator *db) {
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS book_copy (
            copy_id TEXT PRIMARY KEY,
            book_id TEXT NOT NULL,
            status TEXT NOT NULL,
            FOREIGN KEY (book_id) REFERENCES book(id)
        );
    )";

    if (!db->execute(sql)) {
        Logger::getInstance().logError("DatabaseInitializer::createBookCopyTable执行SQL失败:" + db->getLastError());
        return false;
    }
    return true;
}

bool DatabaseInitializer::createRecordTable(DatabaseOperator *db) {
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS record (
            user_id TEXT NOT NULL,
            book_id TEXT NOT NULL,
            copy_id TEXT NOT NULL,
            borrow_time INTEGER NOT NULL,
            return_time INTEGER NOT NULL,
            FOREIGN KEY (user_id) REFERENCES user(user_id),
            FOREIGN KEY (copy_id) REFERENCES book_copy(copy_id)
        );
    )";

    if (!db->execute(sql)) {
        Logger::getInstance().logError("DatabaseInitializer::createRecordTable执行SQL失败:" + db->getLastError());
        return false;
    }
    return true;
}

bool DatabaseInitializer::createBlackListTable(DatabaseOperator *db) {
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS black_list (
            user_id TEXT PRIMARY KEY,
            FOREIGN KEY (user_id) REFERENCES user(user_id)
        );
    )";

    if (!db->execute(sql)) {
        Logger::getInstance().logError("DatabaseInitializer::createBlackListTable执行SQL失败:" + db->getLastError());
        return false;
    }
    return true;
}

bool DatabaseInitializer::createIndexes(DatabaseOperator *db) {
    const std::vector<std::string> indexSQLs = {
        "CREATE INDEX IF NOT EXISTS idx_book_title ON book(title);",
        "CREATE INDEX IF NOT EXISTS idx_book_author ON book(author);",
        "CREATE INDEX IF NOT EXISTS idx_book_category ON book(category);",
        "CREATE INDEX IF NOT EXISTS idx_book_copy_book_id ON book_copy(book_id);",
        "CREATE INDEX IF NOT EXISTS idx_book_copy_status ON book_copy(status);",
        "CREATE INDEX IF NOT EXISTS idx_record_user_id ON record(user_id);",
        "CREATE INDEX IF NOT EXISTS idx_record_copy_id ON record(copy_id);",
        "CREATE INDEX IF NOT EXISTS idx_record_book_id ON record(book_id);"
    };

    for (const auto &sql: indexSQLs) {
        if (!db->execute(sql)) {
            Logger::getInstance().logError("DatabaseInitializer::createIndexes执行SQL失败:" + db->getLastError());
            return false;
        }
    }
    return true;
}
