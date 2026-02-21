cmake_minimum_required(VERSION 3.14)
add_compile_definitions(_WIN32_WINNT=0x0A00)
project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 20)

# 修复 MinGW 汇编器不支持 gdwarf-5 的问题
if (MINGW)
    string(REPLACE "-g" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "-g" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g0")
endif ()

# 添加 SQLite3 库
add_library(sqlite3 STATIC utils/sqlite3.c)
target_include_directories(sqlite3 PUBLIC utils)

# 添加HTTP服务器程序
add_executable(server_main server_main.cpp
        database/DatabaseOperator.cpp
        database/DatabaseOperator.h
        database/DatabaseInitializer.cpp
        database/DatabaseInitializer.h
        database/dao/UserDAO.cpp
        database/dao/UserDAO.h
        database/dao/BookDAO.cpp
        database/dao/BookDAO.h
        database/dao/BookCopyDAO.cpp
        database/dao/BookCopyDAO.h
        database/dao/RecordDAO.cpp
        database/dao/RecordDAO.h
        service/UserService.cpp
        service/UserService.h
        service/InventoryService.cpp
        service/InventoryService.h
        service/SearchService.cpp
        service/SearchService.h
        service/borrowService.cpp
        service/borrowService.h
        controller/user/AuthController.cpp
        controller/user/AuthController.h
        controller/admin/AdminBookController.cpp
        controller/admin/AdminBookController.h
        controller/admin/AdminUserController.cpp
        controller/admin/AdminUserController.h
        controller/Router.cpp
        controller/Router.h
        entity/User.cpp
        entity/User.h
        entity/Book.cpp
        entity/Book.h
        entity/BookCopy.cpp
        entity/BookCopy.h
        entity/Record.cpp
        entity/Record.h
        utils/PasswordUtils.cpp
        utils/PasswordUtils.h
        utils/HttpUtils.cpp
        utils/HttpUtils.h
        database/dao/BlackListDAO.cpp
        database/dao/BlackListDAO.h
        service/BlackListService.cpp
        service/BlackListService.h
        controller/admin/AdminBlackListController.cpp
        controller/admin/AdminBlackListController.h
        #        controller/admin/AdminBatchAddController.cpp
        #        controller/admin/AdminBatchAddController.h
        #        service/BatchAddService.cpp
        #        service/BatchAddService.h
        #        utils/TxtToDatabaseWriter.cpp
        #        utils/TxtToDatabaseWriter.h
        #        utils/SqlToDatabaseWriter.cpp
        #        utils/SqlToDatabaseWriter.h
        #        utils/FileProcessor.cpp
        #        utils/FileProcessor.h
        controller/user/BookController.cpp
        controller/user/BookController.h
        controller/user/UserController.cpp
        controller/user/UserController.h
        utils/TimeUtils.cpp
        utils/TimeUtils.h
        common/Logger.cpp
        common/Logger.h
        controller/admin/AdminController.cpp
        controller/admin/AdminController.h
        #        entity/BatchResult.cpp
        #        entity/BatchResult.h
)

target_include_directories(server_main PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

# 链接SQLite3库和系统库到服务器程序
target_link_libraries(server_main PRIVATE sqlite3 ws2_32 bcrypt)