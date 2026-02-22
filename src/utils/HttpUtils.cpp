#include "utils/HttpUtils.h"

#include <json.hpp>
#include <vector>

using std::vector;
using std::string;

// 解析JSON请求体
nlohmann::json HttpUtils::parseRequestBody(const httplib::Request &req) {
    try {
        return nlohmann::json::parse(req.body);
    } catch (const nlohmann::json::parse_error &e) {
        return nlohmann::json::object();
    }
}

// 验证必填字段
bool HttpUtils::validateRequiredFields(const nlohmann::json &data, const vector<string> &fields, string &errorMsg) {
    for (const auto &field: fields) {
        if (!data.contains(field) || data[field].is_null() ||
            (data[field].is_string() && data[field].get<string>().empty())) {
            errorMsg = "缺少必填字段: " + field;
            return false;
        }
    }
    return true;
}

// 生成成功响应
httplib::Response HttpUtils::createSuccessResponse(const nlohmann::json &data, int statusCode) {
    httplib::Response res;
    res.status = statusCode;
    res.set_content(data.dump(), "application/json; charset=utf-8");
    return res;
}

// 生成错误响应
httplib::Response HttpUtils::createErrorResponse(const string &message, int statusCode) {
    httplib::Response res;
    res.status = statusCode;
    const nlohmann::json errorJson = {
        {"success", false},
        {"message", message}
    };
    res.set_content(errorJson.dump(), "application/json; charset=utf-8");
    return res;
}
