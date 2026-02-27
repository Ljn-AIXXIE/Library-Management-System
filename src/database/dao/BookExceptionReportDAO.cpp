#include "database/dao/BookExceptionReportDAO.h"
#include "common/Logger.h"

#include "httplib.h"

[[nodiscard]] bool
BookExceptionReportDAO::addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const {
    const std::string sql =
            "INSERT INTO book_exception_report (book_copy_id,error_type,exception_description,status,submit_time, reporter_id,handled_time,handler_id)"
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt = nullptr;
    if (!bookExceptionReportDatabase->prepare(sql, &stmt)) {
        Logger::getInstance().logError("BookExceptionReportDAO::addBookExceptionReport()准备SQL失败");
        return false;
    }

    auto now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);

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
            "RecordDAO::addBorrowRecord执行SQL失败:" + bookExceptionReportDatabase->getLastError());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
