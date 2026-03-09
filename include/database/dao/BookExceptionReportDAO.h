#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTDAO_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTDAO_H
#include "database/DatabaseOperator.h"
#include "entities/BookExceptionReport.h"

/*
CREATE TABLE book_exception_report (
     id INTEGER PRIMARY KEY AUTOINCREMENT,
     book_copy_id TEXT NOT NULL,
     error_type TEXT,
     exception_description TEXT NOT NULL,
     status TEXT NOT NULL DEFAULT 'pending'
     CHECK(status IN ('pending','processed')),
     submit_time INTEGER NOT NULL,
     reporter_id TEXT NOT NULL,
     handled_time INTEGER,
     handler_id TEXT,
 );
*/

class BookExceptionReportDAO {
    DatabaseOperator *bookExceptionReportDatabase;

public:
    explicit BookExceptionReportDAO(DatabaseOperator *bookExceptionReportDatabase) : bookExceptionReportDatabase(
        bookExceptionReportDatabase) {
    }

    //核心操作
    [[nodiscard]] bool addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const;

    //校验该异常记录是不是已经存在
    [[nodiscard]] bool isBookExceptionReportExist(const std::string &copyId) const;

    //获取所有的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getAllBookExceptionReport(std::string &errorMassage) const;

    //获取所有未处理的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getAllUnhandledBookExceptionReport(std::string &errorMassage) const;

    //获取指定副本的所有图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getBookExceptionReportByCopyId(
        const std::string &copyId, std::string &errorMassage) const;

    //获取指定副本未处理的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getUnhandledBookExceptionReportByCopyId(
        const std::string &copyId, std::string &errorMassage) const;

    //获取指定提交者的所有图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getBookExceptionReportByReportId(
        const std::string &reportId, std::string &errorMassage) const;

    //获取指定提交者未处理的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getUnhandledBookExceptionReportByReportId(
        const std::string &reportId, std::string &errorMassage) const;

    //获取指定处理者的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getBookExceptionReportByHandlerId(
        const std::string &handlerId, std::string &errorMassage) const;

    //获取图书异常报告的详细信息，用于获取图书异常报告详细页面
    [[nodiscard]] BookExceptionReport getBookExceptionReport(std::string copyId, std::string reporter,
                                                             std::string submitTime, std::string &errorMessage) const;

    [[nodiscard]] bool updateBookExceptionReport(const std::string &copyId, const std::string &reporterId,
                                                 const std::string &submitTime, const std::string &handledId) const;
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTDAO_H
