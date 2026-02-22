#ifndef LIBRARY_MANAGEMENT_SYSTEM_TXTTODATABASEWRITER_H
#define LIBRARY_MANAGEMENT_SYSTEM_TXTTODATABASEWRITER_H

#include <string>

#include "database/dao/BlackListDAO.h"
#include "database/dao/BookCopyDAO.h"
#include "database/dao/BookDAO.h"
#include "database/dao/RecordDAO.h"
#include "database/dao/UserDAO.h"
#include "entities/BatchResult.h"
#include "services/InventoryService.h"

//用于将txt文件中的数据写入数据库
class TxtToDatabaseWriter {
public:
    static BatchResult writeBooksToDatabase(const std::string &data, BookDAO *db, InventoryService *inventoryService);
    static BatchResult writeUsersToDatabase(const std::string &data, UserDAO *db);
    static BatchResult writeBookCopiesToDatabase(const std::string &data, BookCopyDAO *db);
    static BatchResult writeRecordsToDatabase(const std::string &data, RecordDAO *db);
    static BatchResult wireBlackListToDatabase(const std::string &data, BlackListDAO *db);
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_TXTTODATABASEWRITER_H
