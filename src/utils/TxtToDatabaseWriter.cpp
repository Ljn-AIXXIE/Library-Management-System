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

void TxtToDatabaseWriter::writeBooksToDatabase(const string &data, BookDAO *db,
                                               InventoryService *inventoryService,
                                               std::vector<std::string> &errorList) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;

    while (getline(dataStream, line)) {
        lineNumber++;

        try {
            bool rowSuccess = true;
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
            //处理\r
            if (copyCount.back() == '\r') {
                copyCount.pop_back();
            }

            int copyCountInt = stoi(copyCount);

            //书本不存在需要在book表中先添加
            if (!inventoryService->isBookIdExist(bookId)) {
                string errorMessage;
                if (Book book(bookId, title, author, category, publisher, publishDate, price, pages, description); !db->
                    addBook(book, errorMessage)) {
                    errorList.push_back(
                        "第" + std::to_string(lineNumber) + "行记录添加失败,bookId:" + bookId + ",原因:" + errorMessage);
                    rowSuccess = false;
                }
            }

            if (rowSuccess) {
                if (std::string errorMessage; !inventoryService->addCopiesWithTransaction(
                    bookId, copyCountInt, errorMessage)) {
                    errorList.push_back(
                        "第" + std::to_string(lineNumber) + "行记录添加副本失败,bookId:" + bookId + ",原因:" + errorMessage);
                }
            }
        } catch (const std::exception &e) {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录格式错误");
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBooksToDatabase解析行数据失败:" + string(e.what()));
        }
    }
}

void TxtToDatabaseWriter::writeUsersToDatabase(const string &data, UserDAO *db,
                                               std::vector<std::string> &errorList) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;

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
        string borrowCount = row[4];

        //处理\r
        if (borrowCount.back() == '\r') {
            borrowCount.pop_back();
        }

        int borrowCountInt = stoi(borrowCount);


        User user = User(userId, name, password, type, borrowCountInt);

        if (string errorMessage; !db->addUser(user, errorMessage)) {
            if (errorMessage == "Unique constraint failed: user(user_id)") {
                errorMessage = "用户已存在";
            }
            errorList.push_back(
                "第" + std::to_string(lineNumber) + "行记录添加用户失败,userId:" + userId + ",原因:" + errorMessage);
            Logger::getInstance().logError("TxtToDatabaseWriter::writeUsersToDatabase添加用户失败");
        }
    }
}

void TxtToDatabaseWriter::writeBookCopiesToDatabase(const string &data, BookDAO *bookDAO,
                                                    InventoryService *inventoryService,
                                                    std::vector<std::string> &errorList) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;

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

        //处理\r
        if (status.back() == '\r') {
            status.pop_back();
        }

        if (copyId.empty() || bookId.empty() || status.empty()) {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录图书副本信息不完整");
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBookCopiesToDatabase图书副本信息不完整");
            continue;
        }

        if (status != "available" && status != "borrowed") {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录图书副本状态错误");
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBookCopiesToDatabase图书副本状态错误");
            continue;
        }

        if (!bookDAO->isBookIdExist(bookId)) {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录对应的图书不存在");
            continue;
        }

        if (inventoryService->isCopyBookIdExist(copyId)) {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录图书副本已存在");
            continue;
        }

        BookCopy bookCopy(bookId, copyId, status);
        if (string errorMessage; !inventoryService->addCopiesWithTransaction(bookId, 1, errorMessage)) {
            errorList.push_back(
                "第" + std::to_string(lineNumber) + "行记录添加图书副本失败，copyId:" + copyId + ",原因:" + errorMessage);
            Logger::getInstance().logError("TxtToDatabaseWriter::writeBookCopiesToDatabase添加图书副本失败");
        }
    }
}

void TxtToDatabaseWriter::writeRecordsToDatabase(const string &data, RecordDAO *db,
                                                 std::vector<std::string> &errorList) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;

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

        //处理\r
        if (returnTimeStr.back() == '\r') {
            returnTimeStr.pop_back();
        }

        time_t borrowTime = stoll(borrowTimeStr);
        time_t returnTime = stoll(returnTimeStr);

        if (Record record(userId, copyId, borrowTime, returnTime); !db->addBorrowRecord(record)) {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录添加借阅记录失败");
            Logger::getInstance().logError("TxtToDatabaseWriter::writeRecordsToDatabase添加借阅记录失败");
        }
    }
}

void TxtToDatabaseWriter::wireBlackListToDatabase(const string &data, BlackListDAO *db,
                                                  std::vector<std::string> &errorList) {
    stringstream dataStream(data);
    string line;
    getline(dataStream, line);

    int lineNumber = 0;

    while (getline(dataStream, line)) {
        lineNumber++;
        stringstream ss(line);
        string item;
        std::vector<string> row;

        while (getline(ss, item, ',')) {
            row.push_back(item);
        }

        string userId = row[0];

        //处理\r
        if (userId.back() == '\r') {
            userId.pop_back();
        }

        if (!db->addBlackList(userId)) {
            errorList.push_back("第" + std::to_string(lineNumber) + "行记录添加黑名单用户失败");
            Logger::getInstance().logError("TxtToDatabaseWriter::wireBlackListToDatabase添加黑名单用户失败");
        }
    }
}
