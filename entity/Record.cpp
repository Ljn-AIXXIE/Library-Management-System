#include "entity/Record.h"

static int DAYS_TO_KEEP = 30;

Record::Record(const string &userId, const string &copyId, const time_t borrowTime) {
    this->userId = userId;
    this->copyId = copyId;
    this->borrowTime = borrowTime;
    this->returnTime = 0;
    this->dueTime = borrowTime + DAYS_TO_KEEP * 24 * 60 * 60;
}

Record::Record(const string &userId, const string &copyId, const time_t borrowTime, const time_t returnTime) {
    this->userId = userId;
    this->copyId = copyId;
    this->borrowTime = borrowTime;
    this->returnTime = returnTime;
    this->dueTime = borrowTime + DAYS_TO_KEEP * 24 * 60 * 60;
}

Record::~Record() {
}

string Record::getUserId() const {
    return userId;
}

string Record::getCopyId() const {
    return copyId;
}

time_t Record::getBorrowTime() const {
    return borrowTime;
}

time_t Record::getReturnTime() const {
    return returnTime;
}

time_t Record::getDueTime() const {
    return dueTime;
}

void Record::setUserId(string userId) {
    this->userId = userId;
}

void Record::setCopyId(string copyId) {
    this->copyId = copyId;
}

void Record::setBorrowTime(time_t borrowTime) {
    this->borrowTime = borrowTime;
}

void Record::setReturnTime(time_t returnTime) {
    this->returnTime = returnTime;
}

void Record::setDueTime(time_t dueTime) {
    this->dueTime = dueTime;
}
