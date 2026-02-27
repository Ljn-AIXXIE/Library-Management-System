#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTSERVICE_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTSERVICE_H
#include "database/dao/BookExceptionReportDAO.h"

class BookExceptionReportService {
    BookExceptionReportDAO *db;

public:
    explicit BookExceptionReportService(BookExceptionReportDAO *bookExceptionReportDAO) : db(
        bookExceptionReportDAO) {
    }

    bool addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const;
};


#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTSERVICE_H
