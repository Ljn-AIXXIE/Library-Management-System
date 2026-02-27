#ifndef LIBRARY_MANAGEMENT_SYSTEM_DATABASEINITIALIZER_H
#define LIBRARY_MANAGEMENT_SYSTEM_DATABASEINITIALIZER_H

#include "DatabaseOperator.h"

//用于初始化数据库
class DatabaseInitializer {
    // 创建各个表
    static bool createBookTable(DatabaseOperator *db);

    static bool createUserTable(DatabaseOperator *db);

    static bool createBookCopyTable(DatabaseOperator *db);

    static bool createRecordTable(DatabaseOperator *db);

    static bool createBlackListTable(DatabaseOperator *db);

    static bool createBookExceptionReportTable(DatabaseOperator *db);

    static bool createIndexes(DatabaseOperator *db);

public:
    // 初始化数据库，创建所有表
    static bool initializeDatabase(DatabaseOperator *db);
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_DATABASEINITIALIZER_H

