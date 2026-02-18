#ifndef AVL_BOOKSYSTEM_HTTPUTILS_H
#define AVL_BOOKSYSTEM_HTTPUTILS_H
#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "httplib.h"
#include "json.hpp"
#include <vector>
using json = nlohmann::json;
using Response = httplib::Response;

class HttpUtils {
public:
    //解析JSON请求体
    static json parseRequestBody(const httplib::Request &req);

    //验证必填字段
    static bool validateRequiredFields(const json &data, const std::vector<std::string> &fields, std::string &errorMsg);

    //生成成功响应
    static Response createSuccessResponse(const json &data, int statusCode = 200);

    //生成错误响应
    static Response createErrorResponse(const std::string &message, int statusCode = 400);
};


#endif //AVL_BOOKSYSTEM_HTTPUTILS_H
