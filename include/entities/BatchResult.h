#ifndef LIBRARY_MANAGEMENT_SYSTEM_BATCHRESULT_H
#define LIBRARY_MANAGEMENT_SYSTEM_BATCHRESULT_H

class BatchResult {
    int successCount = 0;
    int failureCount = 0;

public:
    BatchResult() = default;
    BatchResult(const int successCount, const int failureCount)
        : successCount(successCount), failureCount(failureCount) {}

    [[nodiscard]] int getSuccessCount() const { return successCount; }
    [[nodiscard]] int getFailureCount() const { return failureCount; }
};

#endif //LIBRARY_MANAGEMENT_SYSTEM_BATCHRESULT_H