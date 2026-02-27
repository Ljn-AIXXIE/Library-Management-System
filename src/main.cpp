#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <httplib.h>

#include "controllers/Router.h"
#include "database/DatabaseOperator.h"
#include "database/DatabaseInitializer.h"
#include "database/dao/BlackListDAO.h"
#include "database/dao/BookCopyDAO.h"
#include "database/dao/BookDAO.h"
#include "database/dao/RecordDAO.h"
#include "database/dao/UserDAO.h"
#include "services/BatchAddService.h"
#include "services/BlackListService.h"
#include "services/BorrowService.h"
#include "services/InventoryService.h"
#include "services/SearchService.h"
#include "services/UserService.h"

using std::cout;
using std::endl;
using std::cerr;

int main() {
    const std::filesystem::path workingPath = std::filesystem::current_path().parent_path() / "assets";
    const std::filesystem::path databasePath = workingPath / "library.db";

#ifdef _WIN32
    // 设置控制台代码页为UTF-8
    system("chcp 65001 > nul");
#endif

    cout << "========================================" << endl;
    cout << "    图书管理系统 - HTTP服务器启动      " << endl;
    cout << "========================================" << endl;
    cout << endl;

    // 1. 初始化数据库
    DatabaseOperator databaseOperator;
    {
        cout << "正在连接数据库..." << endl;
        if (!databaseOperator.open(databasePath.string())) {
            cerr << "✗ 数据库连接失败: " << databaseOperator.getLastError() << endl;
            return 1;
        }
        cout << "✓ 数据库连接成功" << endl;

        cout << "正在检查数据库表..." << endl;
        DatabaseInitializer::initializeDatabase(&databaseOperator);
        cout << endl;
    }

    // 2. 初始化DAO和Service层
    UserDAO userDAO(&databaseOperator);
    BookDAO bookDAO(&databaseOperator);
    BookCopyDAO bookCopyDAO(&databaseOperator);
    BlackListDAO blackListDAO(&databaseOperator);
    RecordDAO recordDAO(&databaseOperator);
    BookExceptionReportDAO bookExceptionReportDAO(&databaseOperator);

    UserService userService(&userDAO);
    InventoryService inventoryService(&bookDAO, &bookCopyDAO, &databaseOperator);
    SearchService searchService(&bookDAO);
    BorrowService borrowService(&bookCopyDAO, &recordDAO, &userDAO);
    BlackListService blackListService(&blackListDAO);
    BatchAddService batchAddService(&bookDAO, &userDAO, &bookCopyDAO, &recordDAO, &blackListDAO, &databaseOperator,
                                    &inventoryService);
    BookExceptionReportService bookExceptionReportService(&bookExceptionReportDAO);

    cout << "✓ Service层初始化完成" << endl;
    cout << endl;

    // 3. 创建HTTP服务器
    httplib::Server server;

    // 4. 初始化路由
    Router router(&server);
    router.initializeRoutes(&userService, &inventoryService, &searchService, &blackListService, &batchAddService,
                            &borrowService, &bookExceptionReportService);
    router.setStaticFileDirectory((workingPath / "public").string());
    cout << endl;

    // 5. 设置服务器参数
    const std::string host = "0.0.0.0";
    constexpr int port = 8080;

    cout << "========================================" << endl;
    cout << "服务器配置信息:" << endl;
    cout << "  地址: http://" << host << ":" << port << endl;
    cout << "  本地访问: http://localhost:" << port << endl;
    cout << "  数据库: " << databasePath << endl;
    cout << "========================================" << endl;
    cout << endl;

    cout << "可用的API端点:" << endl;
    cout << "  POST   /api/auth/register        - 用户注册" << endl;
    cout << "  POST   /api/auth/login           - 用户登录" << endl;
    cout << "  POST   /api/auth/logout          - 用户登出" << endl;
    cout << endl;

    cout << "可用的页面:" << endl;
    cout << "  http://localhost:" << port << "/login.html - 登录页面" << endl;
    cout << endl;

    cout << "========================================" << endl;
    cout << "服务器正在启动..." << endl;
    cout << "按 Ctrl+C 停止服务器" << endl;
    cout << "========================================" << endl;
    cout << endl;

    // 6. 启动服务器
    if (!server.listen(host, port)) {
        cerr << "✗ 服务器启动失败，端口 " << port << " 可能已被占用" << endl;
        return 1;
    }

    cout << endl;
    cout << "服务器已停止" << endl;
}
