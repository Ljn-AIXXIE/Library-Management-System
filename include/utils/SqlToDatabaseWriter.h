#ifndef LIBRARY_MANAGEMENT_SYSTEM_SQLTODATABASEWRITER_H
#define LIBRARY_MANAGEMENT_SYSTEM_SQLTODATABASEWRITER_H

#include <string>

#include "database/DatabaseOperator.h"

//用于将sql文件中的数据写入数据库
class SqlToDatabaseWriter {
public:
    static void writeSqlToDatabase(const std::string &data, DatabaseOperator *db);
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_SQLTODATABASEWRITER_H
