#ifndef LIBRARY_MANAGEMENT_SYSTEM_ADMINBATCHADDCONTROLLER_H
#define LIBRARY_MANAGEMENT_SYSTEM_ADMINBATCHADDCONTROLLER_H

#include <httplib.h>

#include "services/BatchAddService.h"

class AdminBatchAddController {
    BatchAddService *batchAddService;
public:
    explicit AdminBatchAddController(BatchAddService *batchAddService) : batchAddService(batchAddService) {}

    //POST /api/admin/add/batch
    void handleBatchAdd(const httplib::Request &req,
                        httplib::Response &res,
                        const httplib::ContentReader &content_reader) const;
};


#endif //LIBRARY_MANAGEMENT_SYSTEM_ADMINBATCHADDCONTROLLER_H
