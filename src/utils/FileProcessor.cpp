#include "utils/FileProcessor.h"
#include <iostream>

void FileProcessor::startFile(const std::string &filename) {
    currentFileName = filename;
    buffer.clear();
    std::cout << "开始读取文件: " << filename << std::endl;
}

void FileProcessor::processChunk(const char *data, size_t length,
                                 std::function<void(const std::string &filename, const char *data, size_t length)>
                                 processor) const {
    if (processor) {
        // 直接调用处理函数，不存储完整文件
        processor(currentFileName, data, length);
    }
}

void FileProcessor::finishFile(std::function<void(const std::string &filename)> finalizer) const {
    // 如果需要处理缓冲区的剩余数据
    if (!buffer.empty() && finalizer) {
        // 这里可以处理可能跨块的未完成数据
    }

    std::cout << "完成读取文件: " << currentFileName << std::endl;

    if (finalizer) {
        finalizer(currentFileName);
    }
}

const std::string &FileProcessor::getCurrentFileName() const {
    return currentFileName;
}
