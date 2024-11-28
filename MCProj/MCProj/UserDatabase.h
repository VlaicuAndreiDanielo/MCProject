#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>
#include "User.h"

class UserDatabase {
public:
    UserDatabase(const std::string& dbName);
    ~UserDatabase();

    void createTable();
    void addUser(const User& user);
    void updateUserScore(const std::string& username, int newScore);
    void updateUserUpgradePoints(const std::string& username, int newPoints);
    User getUserInfo(const std::string& username);
    void deleteUser(const std::string& username);
    void showAllUsers();
    void clearTable();
    bool userExists(const std::string& username);
    int getUserIdByUsername(const std::string& username);


private:
    sqlite3* db;

    void executeSQL(const std::string& sql);
};