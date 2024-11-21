#include "UserDatabase.h"

UserDatabase::UserDatabase(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
    else {
        std::cout << "Database opened successfully!" << std::endl;
    }
}

UserDatabase::~UserDatabase()
{
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed!" << std::endl;
    }
}

void UserDatabase::createTable()
{
    const std::string createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS User (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            score INTEGER DEFAULT 0,
            upgradePoints INTEGER DEFAULT 0
        );
    )";
    executeSQL(createTableSQL);
}

void UserDatabase::addUser(const User &user)
{
    const std::string insertSQL = "INSERT INTO User (username, password, score, upgradePoints) VALUES ('"
        + user.getUsername() + "', '"
        + user.getPassword() + "', "
        + std::to_string(user.getScore()) + ", "
        + std::to_string(user.getUpgradePoints()) + ");";
    executeSQL(insertSQL);
}

void UserDatabase::updateUserScore(const std::string& username, int newScore)
{
}

void UserDatabase::executeSQL(const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "SQL executed successfully!" << std::endl;
    }
}
