#ifndef LIBRARY_MANAGEMENT_SYSTEM_BOOKCOPY_H
#define LIBRARY_MANAGEMENT_SYSTEM_BOOKCOPY_H

#include <string>

class BookCopy {
    std::string bookId;
    std::string copyId;
    std::string status; //available,borrowed,damaged,lost

public:
    BookCopy() = default;

    BookCopy(std::string bookId, std::string copyId, std::string status = "available")
        : bookId(std::move(bookId)), copyId(std::move(copyId)), status(std::move(status)) {
    }

    [[nodiscard]] const std::string &getBookId() const { return bookId; }
    [[nodiscard]] const std::string &getCopyId() const { return copyId; }
    [[nodiscard]] const std::string &getStatus() const { return status; }

    void setBookId(const std::string &newBookId) { bookId = newBookId; }
    void setCopyId(const std::string &newCopyId) { copyId = newCopyId; }
    void setStatus(const std::string &newStatus) { status = newStatus; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BOOKCOPY_H
