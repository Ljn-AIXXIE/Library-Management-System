#ifndef LIBRARY_MANAGEMENT_SYSTEM_TXTTODATABASEWRITER_H
#define LIBRARY_MANAGEMENT_SYSTEM_TXTTODATABASEWRITER_H

#include <string>

#include "database/dao/BlackListDAO.h"
#include "database/dao/BookDAO.h"
#include "database/dao/RecordDAO.h"
#include "database/dao/UserDAO.h"
#include "services/InventoryService.h"

//用于将txt文件中的数据写入数据库
class TxtToDatabaseWriter {
public:
    static void writeBooksToDatabase(const std::string &data, BookDAO *db, InventoryService *inventoryService,
                                     std::vector<std::string> &errorList);

    static void writeUsersToDatabase(const std::string &data, UserDAO *db, std::vector<std::string> &errorList);

    static void writeBookCopiesToDatabase(const std::string &data, BookDAO *bookDAO,
                                          InventoryService *inventoryService,
                                          std::vector<std::string> &errorList);

    static void writeRecordsToDatabase(const std::string &data, RecordDAO *db,
                                       std::vector<std::string> &errorList);

    static void wireBlackListToDatabase(const std::string &data, BlackListDAO *db,
                                        std::vector<std::string> &errorList);
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_TXTTODATABASEWRITER_H
