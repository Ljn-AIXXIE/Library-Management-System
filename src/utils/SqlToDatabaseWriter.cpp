#include "utils/SqlToDatabaseWriter.h"

#include <fstream>
#include <sstream>
#include "common/Logger.h"

using std::string;
using std::stringstream;

//用于将sql文件中的数据写入数据库
void SqlToDatabaseWriter::writeSqlToDatabase(const string &data, DatabaseOperator *db) {
    stringstream sqlFile(data);
    string sql;

    int successCount = 0;
    int failureCount = 0;

    while (getline(sqlFile, sql)) {
        if (!db->execute(sql)) {
            failureCount++;
            Logger::getInstance().logError("SqlToDatabaseWriter::writeSqlToDatabase执行SQL失败:" + db->getLastError());
            continue;
        }
        successCount++;
    }
}
