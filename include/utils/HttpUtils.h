#ifndef LIBRARY_MANAGEMENT_SYSTEM_HTTPUTILS_H
#define LIBRARY_MANAGEMENT_SYSTEM_HTTPUTILS_H

#include <httplib.h>
#include <json.hpp>
#include <vector>

class HttpUtils {
public:
    //解析JSON请求体
    static nlohmann::json parseRequestBody(const httplib::Request &req);

    //验证必填字段
    static bool validateRequiredFields(const nlohmann::json &data, const std::vector<std::string> &fields, std::string &errorMsg);

    //生成成功响应
    static httplib::Response createSuccessResponse(const nlohmann::json &data, int statusCode = 200);

    //生成错误响应
    static httplib::Response createErrorResponse(const std::string &message, int statusCode = 400);
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_HTTPUTILS_H
