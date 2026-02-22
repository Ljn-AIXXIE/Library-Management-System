#ifndef LIBRARY_MANAGEMENT_SYSTEM_BATCHADDSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_BATCHADDSERVICE_H

#include "entities/BatchResult.h"
#include "utils/TxtToDatabaseWriter.h"

//负责批量添加数据
class BatchAddService {
    BookDAO *bookDAO;
    UserDAO *userDAO;
    BookCopyDAO *bookCopyDAO;
    RecordDAO *recordDAO;
    BlackListDAO *blackListDAO;
    DatabaseOperator *databaseOperator;
    InventoryService *inventoryService;

public:
    BatchAddService(BookDAO *bookDAO, UserDAO *userDAO, BookCopyDAO *bookCopyDAO, RecordDAO *recordDAO,
                    BlackListDAO *blackListDAO, DatabaseOperator *databaseOperator, InventoryService *inventoryService)
        : bookDAO(bookDAO), userDAO(userDAO), bookCopyDAO(bookCopyDAO), recordDAO(recordDAO),
          blackListDAO(blackListDAO), databaseOperator(databaseOperator), inventoryService(inventoryService) {}

    //核心操作
    [[nodiscard]] BatchResult addBooksFromTxt(const char *data, size_t length) const;
    [[nodiscard]] BatchResult addUsersFromTxt(const char *data, size_t length) const;
    [[nodiscard]] BatchResult addBookCopiesFromTxt(const char *data, size_t length) const;
    [[nodiscard]] BatchResult addRecordsFromTxt(const char *data, size_t length) const;
    [[nodiscard]] BatchResult addBlackListFromTxt(const char *data, size_t length) const;
    [[nodiscard]] BatchResult addFromSql(const char *data, size_t length) const;
    [[nodiscard]] bool validateSql(const char *data, size_t length) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BATCHADDSERVICE_H
