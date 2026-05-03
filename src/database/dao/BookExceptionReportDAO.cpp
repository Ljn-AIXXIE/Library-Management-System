#include "database/dao/BookExceptionReportDAO.h"
#include "utils/TimeUtils.h"
#include "common/Logger.h"

#include "httplib.h"

bool BookExceptionReportDAO::addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const {
    const std::string sql =
            "INSERT INTO book_exception_report (book_copy_id,error_type,exception_description,status,submit_time, reporter_id,handled_time,handler_id) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt = nullptr;

    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookExceptionReportDAO::addBookExceptionReport()准备SQL失败");
        return false;
    }

    auto now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);
    const std::string currentTimeStr = TimeUtils::formatTime(currentTime);
    currentTime = TimeUtils::stringToTimeT(currentTimeStr);

    sqlite3_bind_text(stmt, 1, bookExceptionReport.getCopyId().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, bookExceptionReport.getErrorType().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 3, bookExceptionReport.getExceptionDescription().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 4, bookExceptionReport.getStatus().c_str(), -1, nullptr);
    sqlite3_bind_int64(stmt, 5, currentTime);
    sqlite3_bind_text(stmt, 6, bookExceptionReport.getReporterId().c_str(), -1, nullptr);
    sqlite3_bind_int64(stmt, 7, 0);
    sqlite3_bind_text(stmt, 8, bookExceptionReport.getHandlerId().c_str(), -1, nullptr);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError(
            "BookExceptionReportDAO::addBookExceptionReport执行SQL失败:" + bookExceptionReportDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool BookExceptionReportDAO::isBookExceptionReportExist(const std::string &copyId) const {
    const std::string sql =
            "SELECT EXISTS(SELECT 1 FROM book_exception_report WHERE copy_id = ? "
            "AND submit_time IS NOT NULL AND handled_time IS NULL)";
    sqlite3_stmt *stmt = nullptr;

    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookExceptionReportDAO::isBookExceptionReportExist()准备SQL失败");
        return false;
    }

    sqlite3_bind_text(stmt, 1, copyId.c_str(), -1, nullptr);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookExceptionReportDAO::isBookExceptionReportExist执行SQL失败");
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

//辅助函数用于获取列文本
static std::string columnText(sqlite3_stmt *stmt, int col) {
    const unsigned char *text = sqlite3_column_text(stmt, col);
    return text ? reinterpret_cast<const char *>(text) : "";
}

//辅助函数用于从查询结果中提取报告信息
static BookExceptionReport extractBookExceptionReport(sqlite3_stmt *stmt) {
    BookExceptionReport report;
    report.setCopyId(columnText(stmt, 0));
    report.setErrorType(columnText(stmt, 1));
    report.setExceptionDescription(columnText(stmt, 2));
    report.setStatus(columnText(stmt, 3));
    report.setSubmitTime(stoi(columnText(stmt, 4)));
    report.setReporterId(columnText(stmt, 5));
    report.setHandledTime(stoi(columnText(stmt, 6)));
    report.setHandlerId(columnText(stmt, 7));
    return report;
}

std::vector<BookExceptionReport> BookExceptionReportDAO::getAllBookExceptionReport(std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report";
    sqlite3_stmt *stmt = nullptr;

    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getAllBookExceptionReport()准备SQL失败");
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<BookExceptionReport> BookExceptionReportDAO::getAllUnhandledBookExceptionReport(
    std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE handled_time = 0";
    sqlite3_stmt *stmt = nullptr;
    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getAllUnhandledBookExceptionReport()准备SQL失败");
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}


std::vector<BookExceptionReport> BookExceptionReportDAO::getBookExceptionReportByCopyId(
    const std::string &copyId, std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE book_copy_id = ?";
    sqlite3_stmt *stmt = nullptr;

    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getBookExceptionReportByCopyId()准备SQL失败");
        return result;
    }

    sqlite3_bind_text(stmt, 1, copyId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<BookExceptionReport> BookExceptionReportDAO::getUnhandledBookExceptionReportByCopyId(
    const std::string &copyId, std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE book_copy_id = ? AND handled_time = 0";
    sqlite3_stmt *stmt = nullptr;

    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getUnhandledBookExceptionReportByCopyId()准备SQL失败");
        return result;
    }
    sqlite3_bind_text(stmt, 1, copyId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}


std::vector<BookExceptionReport> BookExceptionReportDAO::getBookExceptionReportByReportId(
    const std::string &reportId, std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE reporter_id = ?";
    sqlite3_stmt *stmt = nullptr;

    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getBookExceptionReportByReportId()准备SQL失败");
        return result;
    }
    sqlite3_bind_text(stmt, 1, reportId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<BookExceptionReport> BookExceptionReportDAO::getUnhandledBookExceptionReportByReportId(
    const std::string &reportId, std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE reporter_id = ? AND handled_time = 0";

    sqlite3_stmt *stmt = nullptr;
    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getUnhandledBookExceptionReportByReportId()准备SQL失败");
        return result;
    }
    sqlite3_bind_text(stmt, 1, reportId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<BookExceptionReport> BookExceptionReportDAO::getBookExceptionReportByHandlerId(
    const std::string &handlerId, std::string &errorMassage) const {
    std::vector<BookExceptionReport> result;
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE handler_id = ?";
    sqlite3_stmt *stmt = nullptr;
    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMassage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getBookExceptionReportByHandlerId()准备SQL失败");
        return result;
    }

    sqlite3_bind_text(stmt, 1, handlerId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(extractBookExceptionReport(stmt));
    }
    sqlite3_finalize(stmt);
    return result;
}

BookExceptionReport BookExceptionReportDAO::getBookExceptionReport(std::string copyId, std::string reporter,
                                                                   std::string submitTime,
                                                                   std::string &errorMessage) const {
    std::string sql =
            "SELECT book_copy_id,error_type,exception_description,status,submit_time,reporter_id,handled_time,handler_id "
            "FROM book_exception_report "
            "WHERE book_copy_id = ? AND reporter_id = ? AND submit_time = ?";
    sqlite3_stmt *stmt = nullptr;
    BookExceptionReport result;
    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        errorMessage = bookExceptionReportDatabase->getLastError();
        Logger::getInstance().logError("BookExceptionReportDAO::getBookExceptionReport()");
        return result;
    }

    int submitTimeInt = TimeUtils::stringToTimeT(submitTime);

    sqlite3_bind_text(stmt, 1, copyId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, reporter.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 3, submitTimeInt);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        Logger::getInstance().logError("BookExceptionReportDAO::getBookExceptionReport()执行SQL失败");
        bookExceptionReportDatabase->setLastError(sqlite3_errmsg(bookExceptionReportDatabase->getDB()));
        errorMessage = bookExceptionReportDatabase->getLastError();
        sqlite3_finalize(stmt);
        return result;
    }

    result = extractBookExceptionReport(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool BookExceptionReportDAO::updateBookExceptionReport(const std::string &copyId, const std::string &reporterId,
                                                       const std::string &submitTime,
                                                       const std::string &handledId) const {
    auto now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);
    const std::string currentTimeStr = TimeUtils::formatTime(currentTime);
    currentTime = TimeUtils::stringToTimeT(currentTimeStr);
    int submitTimeInt = TimeUtils::stringToTimeT(submitTime);

    const std::string sql =
            "UPDATE book_exception_report "
            "SET handled_time = ? , handler_id = ? ,status = processed "
            "WHERE book_copy_id = ? AND reporter_id = ? AND submit_time = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookExceptionReportDAO::updateBookExceptionReport()执行SQL失败");
        bookExceptionReportDatabase->setLastError(sqlite3_errmsg(bookExceptionReportDatabase->getDB()));
        std::string errorMessage = bookExceptionReportDatabase->getLastError();
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_bind_int64(stmt, 1, currentTime);
    sqlite3_bind_text(stmt, 2, handledId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, copyId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, reporterId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 5, submitTimeInt);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Logger::getInstance().logError("BookExceptionReportDAO::updateBookExceptionReport() step failed");
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
