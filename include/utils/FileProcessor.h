#ifndef LIBRARYMANAGEMENTSYSTEM_FILEPROCESSOR_H
#define LIBRARYMANAGEMENTSYSTEM_FILEPROCESSOR_H

#include <string>
#include <vector>
#include <functional>

class FileProcessor {
    std::string currentFileName;
    std::vector<char> buffer;

public:
    void startFile(const std::string &filename);

    void processChunk(const char *data, size_t length,
                      std::function<void(const std::string &filename, const char *data,
                                         size_t length)> processor) const;

    void finishFile(std::function<void(const std::string &filename)> finalizer = nullptr) const;

    [[nodiscard]] const std::string& getCurrentFileName() const;
};


#endif //LIBRARYMANAGEMENTSYSTEM_FILEPROCESSOR_H
