#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>

class UserDatabase {
public:
    UserDatabase(const std::string& dbName);
    ~UserDatabase();

    void createTable();
    void addUser(const std::string& username, const std::string& password, int score, int upgradePoints);
    void updateUserScore(const std::string& username, int newScore);
    void updateUserUpgradePoints(const std::string& username, int newPoints);
    void getUserInfo(const std::string& username);
    void deleteUser(const std::string& username);

private:
    sqlite3* db;

    void executeSQL(const std::string& sql);
};