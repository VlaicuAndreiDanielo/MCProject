#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>
#include "User.h"

class UserDatabase {
public:
    UserDatabase(const std::string& dbName);
    ~UserDatabase();

    void CreateTable();
    void AddUser(const User& user);
    void UpdateUserScore(const std::string& username, int newScore);
    void UpdateUserUpgradePoints(const std::string& username, int newPoints);
    User GetUserInfo(const std::string& username);
    void DeleteUser(const std::string& username);
    void ShowAllUsers();
    void ClearTable();
    bool UserExists(const std::string& username);
    int GetUserIdByUsername(const std::string& username);
    bool AuthenticateUser(const std::string& username, const std::string& password);
    std::string GetUsernameById(int userId);

private:
    sqlite3* m_db;

    void ExecuteSQL(const std::string& sql);
};