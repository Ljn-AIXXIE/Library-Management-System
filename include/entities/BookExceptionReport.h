#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORT_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORT_H
#include <string>
#include <utility>

class BookExceptionReport {
    std::string copyId;
    std::string reporterId;
    std::string errorType;
    std::string exceptionDescription;
    std::string status;
    time_t submitTime;
    time_t handledTime;
    std::string handlerId;

public:
    BookExceptionReport() {
        copyId = "";
        reporterId = "";
        errorType = "";
        exceptionDescription = "";
        status = "";
        submitTime = 0;
        handledTime = 0;
        handlerId = "";
    }

    BookExceptionReport(std::string copyId, std::string reporterId, std::string errorType,
                        std::string exceptionDescription) : copyId(std::move(copyId)),
                                                            reporterId(std::move(reporterId)),
                                                            errorType(std::move(errorType)),
                                                            exceptionDescription(
                                                                std::move(exceptionDescription)) {
        status = "pending";
        submitTime = 0;
        handledTime = 0;
    } //用于数据库添加

    BookExceptionReport(std::string copyId, std::string reporterId, std::string errorType,
                        std::string exceptionDescription,
                        std::string status, const int submitTime, const int handledTime,
                        std::string handlerId) : copyId(std::move(copyId)),
                                                 reporterId(std::move(reporterId)), errorType(std::move(errorType)),
                                                 exceptionDescription(std::move(exceptionDescription)),
                                                 status(std::move(status)),
                                                 submitTime(submitTime),
                                                 handledTime(handledTime), handlerId(std::move(handlerId)) {
    } //用于数据库批量导入

    void setCopyId(std::string newCopyId) { copyId = std::move(newCopyId); }
    void setReporterId(std::string newReporterId) { reporterId = std::move(newReporterId); }
    void setHandlerId(std::string newHandlerId) { handlerId = std::move(newHandlerId); }
    void setErrorType(std::string newErrorType) { errorType = std::move(newErrorType); }

    void setExceptionDescription(std::string newExceptionDescription) {
        exceptionDescription = std::move(newExceptionDescription);
    }

    void setStatus(std::string newStatus) { status = std::move(newStatus); }
    void setSubmitTime(const int newSubmitTime) { submitTime = newSubmitTime; }
    void setHandledTime(const int newHandledTime) { handledTime = newHandledTime; }

    [[nodiscard]] std::string getCopyId() const { return copyId; }
    [[nodiscard]] std::string getReporterId() const { return reporterId; }
    [[nodiscard]] std::string getErrorType() const { return errorType; }
    [[nodiscard]] std::string getExceptionDescription() const { return exceptionDescription; }
    [[nodiscard]] std::string getStatus() const { return status; }
    [[nodiscard]] int getSubmitTime() const { return submitTime; }
    [[nodiscard]] int getHandledTime() const { return handledTime; }
    [[nodiscard]] std::string getHandlerId() const { return handlerId; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKEXCEPTIONREPORT_H
