#include "controllers/admin/AdminBatchAddController.h"
#include <fstream>
#include "utils/HttpUtils.h"

void AdminBatchAddController::handleBatchAdd(const httplib::Request &req, httplib::Response &res,
                                             const httplib::ContentReader &content_reader) const {
    bool hasError = false;
    bool addErrorReport = false;
    std::string errorMessage;

    std::string currentFilename;
    std::string fileBuffer;

    content_reader(
        [&](const httplib::FormData &file_info) {
            if (file_info.name == "file") {
                if (file_info.filename.empty()) {
                    hasError = true;
                    errorMessage = "文件名为空";
                    return false;
                }
                currentFilename = file_info.filename;
                fileBuffer.clear();
            }

            return true;
        },

        [&](const char *data, size_t data_length) {
            if (hasError) return false;
            fileBuffer.append(data, data_length);
            return true;
        });

    if (currentFilename.empty()) {
        const nlohmann::json responseData = {
            {"success", false},
            {"message", "未上传文件"}
        };
        res = HttpUtils::createErrorResponse(responseData, 400);
        return;
    }

    //文件大小限制
    constexpr size_t MAX_FILE_SIZE = 10 * 1024 * 1024; // 10MB
    if (fileBuffer.size() > MAX_FILE_SIZE) {
        nlohmann::json responseData = {
            {"success", false},
            {"message", "文件大小超过限制"}
        };
        res = HttpUtils::createErrorResponse(responseData, 400);
        return;
    }

    //扩展名统一转小写
    std::string filename = currentFilename;
    std::ranges::transform(filename, filename.begin(), ::tolower);

    std::vector<std::string> errorList; //用于生成错误报告

    if (!hasError) {
        try {
            std::string firstLine = fileBuffer.substr(0, fileBuffer.find_first_of("\r\n"));
            if (filename.ends_with(".txt")) {
                // 校验 TXT
                if (firstLine == "id,title,author,category,publisher,publish_date,price,pages,description,copy_count") {
                    batchAddService->addBooksFromTxt(fileBuffer.data(), fileBuffer.size(), errorList);
                    if (!errorList.empty()) {
                        hasError = true;
                        addErrorReport = true;
                        errorMessage = "部分图书添加失败";
                    }
                } else if (firstLine == "user_id,name,role,password,borrow_count") {
                    batchAddService->addUsersFromTxt(fileBuffer.data(), fileBuffer.size(), errorList);
                    if (!errorList.empty()) {
                        hasError = true;
                        addErrorReport = true;
                        errorMessage = "部分用户添加失败";
                    }
                } else if (firstLine == "copy_id,book_id,status") {
                    batchAddService->addBookCopiesFromTxt(fileBuffer.data(), fileBuffer.size(), errorList);
                    if (!errorList.empty()) {
                        hasError = true;
                        addErrorReport = true;
                        errorMessage = "部分图书副本添加失败";
                    }
                } else if (firstLine == "user_id,copy_id,borrow_time,return_time") {
                    batchAddService->addRecordsFromTxt(fileBuffer.data(), fileBuffer.size(), errorList);
                    if (!errorList.empty()) {
                        hasError = true;
                        addErrorReport = true;
                        errorMessage = "部分借阅记录添加失败";
                    }
                } else if (firstLine == "user_id") {
                    batchAddService->addBlackListFromTxt(fileBuffer.data(), fileBuffer.size(), errorList);
                    if (!errorList.empty()) {
                        hasError = true;
                        addErrorReport = true;
                        errorMessage = "部分黑名单用户添加失败";
                    }
                } else {
                    hasError = true;
                    errorMessage = "文本文件格式不正确";
                }
            } else if (filename.ends_with(".sql")) {
                if (batchAddService->validateSql(fileBuffer.data(), fileBuffer.size())) {
                    batchAddService->addFromSql(fileBuffer.data(), fileBuffer.size(), errorList);
                    if (!errorList.empty()) {
                        hasError = true;
                        addErrorReport = true;
                        errorMessage = "执行SQL失败";
                    }
                } else {
                    hasError = true;
                    errorMessage = "SQL文件格式不正确";
                }
            } else {
                hasError = true;
                errorMessage = "不支持的文件类型: " + currentFilename;
            }
        } catch (const std::exception &e) {
            hasError = true;
            errorMessage = "文件处理失败:" + std::string(e.what());
        }
    }

    if (hasError) {
        if (addErrorReport) {
            std::string report;

            report += "错误报告:\n";
            report += "----------------------------------------\n";
            for (const auto &error: errorList) {
                report += error + "\n";
            }
            res.set_header("Content-Disposition", "attachment; filename=error_report.txt");
            res.set_content(report, "text/plain");
            return;
        }
        nlohmann::json responseData = {
            {"success", false},
            {"message", errorMessage}
        };
        res = HttpUtils::createErrorResponse(responseData, 400);
    } else {
        nlohmann::json responseData = {
            {"success", true},
            {"message", "批量导入成功"}
        };
        res = HttpUtils::createSuccessResponse(responseData, 200);
    }
}
