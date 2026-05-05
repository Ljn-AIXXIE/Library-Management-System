cmake_minimum_required(VERSION 4.1)
project(Library_Management_System)
add_compile_definitions(_WIN32_WINNT=0x0A00)

set(CMAKE_CXX_STANDARD 20)
set(LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/external/libraries)

add_executable(Library_Management_System
        include/common/Logger.h
        include/controllers/admin/AdminBatchAddController.h
        include/controllers/admin/AdminBlackListController.h
        include/controllers/admin/AdminBookController.h
        include/controllers/admin/AdminBookExceptionController.h
        include/controllers/admin/AdminController.h
        include/controllers/admin/SuperAdminController.h
        include/controllers/user/AuthController.h
        include/controllers/user/BookController.h
        include/controllers/user/UserController.h
        include/controllers/Router.h
        include/database/DatabaseOperator.h
        include/database/DatabaseInitializer.h
        include/database/dao/BlackListDAO.h
        include/database/dao/BookCopyDAO.h
        include/database/dao/BookDAO.h
        include/database/dao/BookExceptionReportDAO.h
        include/database/dao/RecordDAO.h
        include/database/dao/UserDAO.h
        include/entities/Book.h
        include/entities/BookCopy.h
        include/entities/BookExceptionReport.h
        include/entities/Record.h
        include/entities/User.h
        include/services/BatchAddService.h
        include/services/BlackListService.h
        include/services/BookExceptionReportService.h
        include/services/BorrowService.h
        include/services/InventoryService.h
        include/services/SearchService.h
        include/services/UserService.h
        include/utils/FileProcessor.h
        include/utils/HttpUtils.h
        include/utils/PasswordUtils.h
        include/utils/SqlToDatabaseWriter.h
        include/utils/TimeUtils.h
        include/utils/TxtToDatabaseWriter.h

        src/common/Logger.cpp
        src/controllers/admin/AdminBatchAddController.cpp
        src/controllers/admin/AdminBlackListController.cpp
        src/controllers/admin/AdminBookController.cpp
        src/controllers/admin/AdminBookExceptionController.cpp
        src/controllers/admin/AdminController.cpp
        src/controllers/admin/AdminUserController.cpp
        src/controllers/admin/SuperAdminController.cpp
        src/controllers/user/AuthController.cpp
        src/controllers/user/BookController.cpp
        src/controllers/user/UserController.cpp
        src/controllers/Router.cpp
        src/database/DatabaseInitializer.cpp
        src/database/DatabaseOperator.cpp
        src/database/dao/BlackListDAO.cpp
        src/database/dao/BookCopyDAO.cpp
        src/database/dao/BookDAO.cpp
        src/database/dao/BookExceptionReportDAO.cpp
        src/database/dao/RecordDAO.cpp
        src/database/dao/UserDAO.cpp
        src/services/BatchAddService.cpp
        src/services/BlackListService.cpp
        src/services/BookExceptionReportService.cpp
        src/services/BorrowService.cpp
        src/services/InventoryService.cpp
        src/services/SearchService.cpp
        src/services/UserService.cpp
        src/utils/FileProcessor.cpp
        src/utils/HttpUtils.cpp
        src/utils/PasswordUtils.cpp
        src/utils/SqlToDatabaseWriter.cpp
        src/utils/TxtToDatabaseWriter.cpp
        src/main.cpp
)

target_link_libraries(Library_Management_System PRIVATE
        bcrypt ws2_32
        ${LIBRARIES_DIR}/libsqlite3.a)

target_include_directories(Library_Management_System PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include)
target_include_directories(Library_Management_System PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}" LIBRARY_PROJECT_ROOT_NORMALIZED)
target_compile_definitions(Library_Management_System PRIVATE
        "LIBRARY_PROJECT_ROOT_STR=\"${LIBRARY_PROJECT_ROOT_NORMALIZED}\"")