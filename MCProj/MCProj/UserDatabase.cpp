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
    const std::string insertSQL = "INSERT INTO User (username, password, score, upgradePoints) VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user.getScore());
    sqlite3_bind_int(stmt, 4, user.getUpgradePoints());

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }
    std::cout << "User added with succes!\n";
    sqlite3_finalize(stmt);

}

void UserDatabase::updateUserScore(const std::string& username, int newScore)
{
    const std::string updateSQL = "UPDATE User SET score = ? WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, newScore);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void UserDatabase::updateUserUpgradePoints(const std::string& username, int newPoints)
{
    const std::string updateSQL = "UPDATE User SET upgradePoints = ? WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, newPoints);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

User UserDatabase::getUserInfo(const std::string& username)
{
    const std::string querySQL = "SELECT * FROM User WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    User user("", "", 0, 0);  // Obiect User gol
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user.setUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        user.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        user.setScore(sqlite3_column_int(stmt, 3));
        user.setUpgradePoints(sqlite3_column_int(stmt, 4));
    }
    else {
        throw std::runtime_error("User not found!");
    }

    sqlite3_finalize(stmt);
    return user;
}

void UserDatabase::deleteUser(const std::string& username)
{
    const std::string deleteSQL = "DELETE FROM User WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, deleteSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void UserDatabase::showAllUsers()
{
    const std::string querySQL = "SELECT * FROM User;"; // Query pentru a prelua toți utilizatorii
    sqlite3_stmt* stmt;

    // Pregătește interogarea
    int rc = sqlite3_prepare_v2(db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query: " + std::string(sqlite3_errmsg(db)));
    }

    std::cout << "Users in database:\n";
    std::cout << "---------------------\n";

    // Parcurge rezultatele interogării
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Extrage valorile din fiecare coloană
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));  // Coloana 1 - username
        std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));  // Coloana 2 - password
        int score = sqlite3_column_int(stmt, 3);    // Coloana 3 - score
        int upgradePoints = sqlite3_column_int(stmt, 4); // Coloana 4 - upgradePoints

        // Afișează datele
        std::cout << "Username: " << username << ", Password: " << password
            << ", Score: " << score << ", Upgrade Points: " << upgradePoints << "\n";
    }

    // Finalizează interogarea pentru a elibera resursele
    sqlite3_finalize(stmt);
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
