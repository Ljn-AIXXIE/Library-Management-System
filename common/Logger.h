#ifndef LIBRARYMANAGEMENTSYSTEM_LOGGER_H
#define LIBRARYMANAGEMENTSYSTEM_LOGGER_H
#pragma once
#include <string>
#include <fstream>
#include <mutex>
using namespace std;

class Logger {
    Logger();

    ofstream accessFile;
    ofstream businessFile;
    ofstream errorFile;
    mutex logMutex;

    static string getCurrentTimestamp() ;

public:
    static Logger &getInstance();

    void logAccess(const string &message);

    void logBusiness(const string &message);

    void logError(const string &message);
};


#endif //LIBRARYMANAGEMENTSYSTEM_LOGGER_H
