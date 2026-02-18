#ifndef AVL_BOOKSYSTEM_RECORD_H
#define AVL_BOOKSYSTEM_RECORD_H
#include <string>
#include <chrono>
using std::string;

class Record {
    string userId;
    string copyId;
    time_t borrowTime;
    time_t returnTime;
    time_t dueTime;

public:
    Record(const string &userId, const string &copyId, time_t borrowTime);

    Record(const string &userId, const string &copyId, time_t borrowTime, time_t returnTime);

    ~Record();

    string getUserId() const;

    string getCopyId() const;

    time_t getBorrowTime() const;

    time_t getReturnTime() const;

    time_t getDueTime() const;

    void setUserId(string userId);

    void setCopyId(string copyId);

    void setBorrowTime(time_t borrowTime);

    void setReturnTime(time_t returnTime);

    void setDueTime(time_t dueTime);
};


#endif //AVL_BOOKSYSTEM_RECORD_H
