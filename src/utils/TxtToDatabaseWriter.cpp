#include "utils/TxtToDatabaseWriter.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "common/Logger.h"
#include "database/dao/BookCopyDAO.h"

using std::string;
using std::cout;
using std::endl;
using std::stringstream;
using std::istringstream;

BatchResult TxtToDatabaseWriter::writeBooksToDatabase(const string &data, BookDAO *db,
                                                      InventoryService *inventoryService) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int successCount = 0;
    int failureCount = 0;
    int lineNumber = 0;

    while (getline(dataStream, line)) {
        lineNumber++;
        bool rowSuccess = true;

        try {
            stringstream ss(line);
            string item;
            std::vector<string> row;

            while (getline(ss, item, ',')) {
                row.push_back(item);
            }

            string bookId = row[0];
            string title = row[1];
            string author = row[2];
            string category = row[3];
            string publisher = row[4];
            string publishDate = row[5];
            string price = row[6];
            string pages = row[7];
            string description = row[8];
            string copyCount = row[9];

            int copyCountInt = stoi(copyCount);

            Book book(bookId, title, author, category, publisher, publishDate, price, pages, description);
            if (!db->addBook(book)) {
                rowSuccess = false;
            }
            if (rowSuccess) {
                for (int i = 0; i < copyCountInt; i++) {
                    string copyId = inventoryService->generateCopyId(bookId);

                    if (!inventoryService->updateBookCopyCount(bookId)) {
                        rowSuccess = false;
                        Logger::getInstance().logError("TxtToDatabaseWriter::writeBooksToDatabase更新图书副本数量失败");
                        break;
                    }

                    BookCopy bookCopy(bookId, copyId, "available");

                    if (!inventoryService->addBookCopy(bookCopy)) {
                        rowSuccess = false;
                        Logger::getInstance().logError("TxtToDatabaseWriter::writeBooksToDatabase添加图书副本失败");
                        break;
                    }
                }
            }

            if (rowSuccess) successCount++;
            else failureCount++;
        } catch (const std::exception &e) {
            failureCount++;
            cout << "第" << lineNumber << "行异常:" << e.what() << endl;
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBooksToDatabase解析行数据失败:" + string(e.what()));
        }
    }
    BatchResult result(successCount, failureCount);
    return result;
}

BatchResult TxtToDatabaseWriter::writeUsersToDatabase(const string &data, UserDAO *db) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;
    int successCount = 0;
    int failureCount = 0;

    while (getline(dataStream, line)) {
        lineNumber++;

        stringstream ss(line);
        string item;
        std::vector<string> row;

        while (getline(ss, item, ',')) {
            row.push_back(item);
        }

        string userId = row[0];
        string name = row[1];
        string password = row[2]; //加密过后的密码
        string type = row[3];

        User user = User(userId, name, password, type);

        if (!db->addUser(user)) {
            Logger::getInstance().logError("TxtToDatabaseWriter::writeUsersToDatabase添加用户失败");
            failureCount++;
            continue;
        }
        successCount++;
    }
    BatchResult result(successCount, failureCount);
    return result;
}

BatchResult TxtToDatabaseWriter::writeBookCopiesToDatabase(const string &data, BookCopyDAO *db) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;
    int successCount = 0;
    int failureCount = 0;

    while (getline(dataStream, line)) {
        lineNumber++;

        stringstream ss(line);
        string item;
        std::vector<string> row;

        while (getline(ss, item, ',')) {
            row.push_back(item);
        }
        string copyId = row[0];
        string bookId = row[1];
        string status = row[2];

        if (copyId.empty() || bookId.empty() || status.empty()) {
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBookCopiesToDatabase图书副本信息不完整");
            failureCount++;
            continue;
        }

        BookCopy bookCopy(bookId, copyId, status);
        if (!db->addBookCopy(bookCopy)) {
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBookCopiesToDatabase添加图书副本失败");
            failureCount++;
            continue;
        }
        successCount++;
    }
    BatchResult result(successCount, failureCount);
    return result;
}

BatchResult TxtToDatabaseWriter::writeRecordsToDatabase(const string &data, RecordDAO *db) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;
    int successCount = 0;
    int failureCount = 0;

    while (getline(dataStream, line)) {
        lineNumber++;
        stringstream ss(line);
        string item;
        std::vector<string> row;

        while (getline(ss, item, ',')) {
            row.push_back(item);
        }

        string userId = row[0];
        string copyId = row[1];
        string borrowTimeStr = row[2];
        string returnTimeStr = row[3];

        time_t borrowTime = stoll(borrowTimeStr);
        time_t returnTime = stoll(returnTimeStr);

        Record record(userId, copyId, borrowTime, returnTime);
        if (!db->addBorrowRecord(record)) {
            Logger::getInstance().logError("TxtToDatabaseWriter::writeRecordsToDatabase添加借阅记录失败");
            failureCount++;
            continue;
        }
        successCount++;
    }
    BatchResult result(successCount, failureCount);
    return result;
}

BatchResult TxtToDatabaseWriter::wireBlackListToDatabase(const string &data, BlackListDAO *db) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;
    int successCount = 0;
    int failureCount = 0;

    while (getline(dataStream, line)) {
        lineNumber++;
        stringstream ss(line);
        string item;
        std::vector<string> row;

        while (getline(ss, item, ',')) {
            row.push_back(item);
        }

        string userId = row[0];

        if (!db->addBlackList(userId)) {
            Logger::getInstance().logError("TxtToDatabaseWriter::wireBlackListToDatabase添加黑名单用户失败");
            failureCount++;
            continue;
        }
        successCount++;
    }
    BatchResult result(successCount, failureCount);
    return result;
}
