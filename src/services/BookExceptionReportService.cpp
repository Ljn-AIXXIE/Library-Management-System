#include "services/BookExceptionReportService.h"

bool BookExceptionReportService::addBookExceptionReport(const BookExceptionReport &bookExceptionReport) const {
    if (db->addBookExceptionReport(bookExceptionReport)) {
        return true;
    }
    return false;
}

bool BookExceptionReportService::isBookExceptionReportExist(const std::string &copyId) const {
    if (db->isBookExceptionReportExist(copyId)) {
        return true;
    }
    return false;
}

std::vector<BookExceptionReport>
BookExceptionReportService::getAllBookExceptionReport(std::string &errorMassage, const bool Unhandled) const {
    if (Unhandled) {
        return db->getAllUnhandledBookExceptionReport(errorMassage);
    }
    return db->getAllBookExceptionReport(errorMassage);
}

std::vector<BookExceptionReport> BookExceptionReportService::getBookExceptionReportByCopyId(
    const std::string &copyId, std::string &errorMassage, const bool Unhandled) const {
    if (Unhandled) {
        return db->getUnhandledBookExceptionReportByCopyId(copyId, errorMassage);
    }
    return db->getBookExceptionReportByCopyId(copyId, errorMassage);
}

std::vector<BookExceptionReport> BookExceptionReportService::getBookExceptionReportByReporterId(
    const std::string &reportId, std::string &errorMassage, const bool Unhandled) const {
    if (Unhandled) {
        return db->getUnhandledBookExceptionReportByReportId(reportId, errorMassage);
    }
    return db->getBookExceptionReportByReportId(reportId, errorMassage);
}

std::vector<BookExceptionReport> BookExceptionReportService::getBookExceptionReportByHandlerId(
    const std::string &handlerId, std::string &errorMassage) const {
    return db->getBookExceptionReportByHandlerId(handlerId, errorMassage);
}

BookExceptionReport BookExceptionReportService::getDetailBookExceptionReport(
    const std::string &copyId, const std::string &reporter, const std::string &submitTime,
    std::string &errorMessage) const {
    return db->getBookExceptionReport(copyId, reporter, submitTime, errorMessage);
}

bool BookExceptionReportService::handleBookExceptionReport(const std::string &copyId, const std::string &reporterId,
                                                           const std::string &submitTime,
                                                           const std::string &handledId) const {
    databaseOperator->beginTransaction();

    if (!copyDAO->updateBookCopyStatus(copyId, "available")) {
        databaseOperator->rollback();
        return false;
    }

    if (!db->updateBookExceptionReport(copyId, reporterId, submitTime, handledId)) {
        databaseOperator->rollback();
        return false;
    }

    databaseOperator->commit();
    return true;
}
