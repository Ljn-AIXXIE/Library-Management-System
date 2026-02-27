#ifndef LIBRARY_MANAGEMENT_SYSTEM_ADMINBOOKEXCEPTIONCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_ADMINBOOKEXCEPTIONCONTROLLER_H
#include "services/BookExceptionReportService.h"
#include "services/InventoryService.h"
#include "utils/HttpUtils.h"

class AdminBookExceptionController {
    BookExceptionReportService *bookExceptionReportService;
    InventoryService *inventoryService;

public:
    explicit
    AdminBookExceptionController(BookExceptionReportService *bookExceptionReportService,
                                 InventoryService *inventoryService) : bookExceptionReportService(
                                                                           bookExceptionReportService),
                                                                       inventoryService(inventoryService) {
    }

    //POST /api/exception/submit - 提交异常
    void handleAddException(const httplib::Request &req, httplib::Response &res) const;
};
#endif //LIBRARY_MANAGEMENT_SYSTEM_ADMINBOOKEXCEPTIONCONTROLLER_H
