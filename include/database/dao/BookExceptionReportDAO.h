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
};


#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORTDAO_H
