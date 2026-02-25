#ifndef LIBRARY_MANAGEMENT_SYSTEM_BATCHADDSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_BATCHADDSERVICE_H

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
          blackListDAO(blackListDAO), databaseOperator(databaseOperator), inventoryService(inventoryService) {
    }

    //核心操作
    void addBooksFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const;

    void addUsersFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const;

    void addBookCopiesFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const;

    void addRecordsFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const;

    void addBlackListFromTxt(const char *data, size_t length, std::vector<std::string> &errorList) const;

    void addFromSql(const char *data, size_t length, std::vector<std::string> &errorList) const;

    [[nodiscard]] bool validateSql(const char *data, size_t length) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BATCHADDSERVICE_H
