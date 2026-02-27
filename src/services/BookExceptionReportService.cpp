#include "services/BookExceptionReportService.h"

bool BookExceptionReportService::addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const {
    if (db->addBookExceptionReport(bookExceptionReport)) {
        return true;
    }
    return false;
}
