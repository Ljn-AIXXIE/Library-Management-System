#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTSERVICE_H
#include "database/dao/BookExceptionReportDAO.h"
#include "database/dao/BookCopyDAO.h"

class BookExceptionReportService {
    BookExceptionReportDAO *db;
    BookCopyDAO *copyDAO;
    DatabaseOperator *databaseOperator;

public:
    explicit BookExceptionReportService(BookExceptionReportDAO *bookExceptionReportDAO, BookCopyDAO *copyDao,
                                        DatabaseOperator *databaseOperator) : db(
                                                                                  bookExceptionReportDAO),
                                                                              copyDAO(copyDao),
                                                                              databaseOperator(databaseOperator) {
    }

    //用于添加书本异常报告
    [[nodiscard]] bool addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const;

    //用于检查该书本异常报告是否已经有了
    [[nodiscard]] bool isBookExceptionReportExist(const std::string &copyId) const;

    //获取所有的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getAllBookExceptionReport(
        std::string &errorMassage, bool Unhandled) const;

    //获取指定副本的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getBookExceptionReportByCopyId(
        const std::string &copyId, std::string &errorMassage, bool Unhandled) const;

    //获取指定管理员提交的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getBookExceptionReportByReporterId(
        const std::string &reportId, std::string &errorMassage, bool Unhandled) const;

    //获取指定管理员处理的图书异常报告
    [[nodiscard]] std::vector<BookExceptionReport> getBookExceptionReportByHandlerId(
        const std::string &handlerId, std::string &errorMassage) const;

    //获取详细的异常报告
    [[nodiscard]] BookExceptionReport getDetailBookExceptionReport(const std::string &copyId,
                                                                   const std::string &reporter,
                                                                   const std::string &submitTime,
                                                                   std::string &errorMessage) const;

    //解决图书异常报告
    [[nodiscard]] bool handleBookExceptionReport(const std::string &copyId, const std::string &reporter,
                                                 const std::string &submitTime, const std::string &handledId) const;
};


#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTSERVICE_H
