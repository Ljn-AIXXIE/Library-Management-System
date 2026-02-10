#include "BlackListService.h"

BlackListService::BlackListService(BlackListDAO *db) : db(db) {
}

BlackListService::~BlackListService() = default;

bool BlackListService::addBlackList(const string &userId) const {
    return db->addBlackList(userId);
}

bool BlackListService::removeBlackList(const string &userId) const {
    return db->removeBlackList(userId);
}

bool BlackListService::isBlackListed(const string &userId) const {
    return db->isBlackListed(userId);
}
