#include "UserDatabase.h"

UserDatabase::UserDatabase(const std::string& dbName)
{
    int result = sqlite3_open(dbName.c_str(), &db);

    if (result != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
    else {
        std::cout << "Database opened successfully: " << dbName << std::endl;

        // Asigură-te că tabela este creată
        try {
            createTable();
        }
        catch (const std::exception& e) {
            std::cerr << "Exception while creating table: " << e.what() << std::endl;
            sqlite3_close(db); // Închide baza de date în caz de eroare
            db = nullptr;
        }
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

    std::cout << "Creating table 'User' if it does not exist..." << std::endl;

    executeSQL(createTableSQL);
}

void UserDatabase::addUser(const User& user)
{
    // Validare de bază pentru username și password
    if (!user.isUsernameValid() || !user.isPasswordValid()) {
        std::cerr << "Invalid user. Cannot add to database." << std::endl;
        return;
    }

    const std::string& username = user.getUsername();
    const std::string& password = user.getPassword();

    if (username.empty() || password.empty()) {
        std::cerr << "Error: Username or password cannot be empty!" << std::endl;
        return;
    }

    // Verifică dacă utilizatorul există deja
    if (userExists(username)) {
        std::cerr << "Error: User with username '" << username << "' already exists in the database." << std::endl;
        return;
    }

    // Debugging: Afișează valorile utilizatorului
    std::cout << "Adding User - Username: " << user.getUsername()
        << ", Password: " << user.getPassword()
        << ", Score: " << user.getScore()
        << ", Upgrade Points: " << user.getUpgradePoints() << std::endl;

    const std::string insertSQL = "INSERT INTO User (username, password, score, upgradePoints) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    // Pregătirea interogării
    int rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind pentru valorile utilizatorului
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user.getScore());
    sqlite3_bind_int(stmt, 4, user.getUpgradePoints());

    // Execută interogarea
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        std::cerr << "Failed SQL: " << insertSQL << std::endl;
    }
    else {
        std::cout << "User added successfully!" << std::endl;
    }

    // Finalizează statement-ul
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
    const std::string querySQL = "SELECT * FROM User;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query: " + std::string(sqlite3_errmsg(db)));
    }

    std::cout << "Users in database:\n";
    std::cout << "---------------------\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int score = sqlite3_column_int(stmt, 3);
        int upgradePoints = sqlite3_column_int(stmt, 4);

        std::cout << "ID: " << id
            << ", Username: " << (username ? username : "NULL")
            << ", Password: " << (password ? password : "NULL")
            << ", Score: " << score
            << ", Upgrade Points: " << upgradePoints << "\n";
    }

    sqlite3_finalize(stmt);
}


bool UserDatabase::userExists(const std::string& username)
{
    const std::string querySQL = "SELECT COUNT(*) FROM User WHERE username = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int exists = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    std::cout << "User '" << username << "' exists: " << (exists > 0 ? "Yes" : "No") << std::endl;
    return exists > 0;
}


void UserDatabase::executeSQL(const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << sql << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "SQL executed successfully: " << sql << std::endl;
    }
}

void UserDatabase::clearTable()
{
    const std::string clearTableSQL = "DELETE FROM User;";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(db, clearTableSQL.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error while clearing table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "All rows deleted from the table successfully!" << std::endl;
    }

    // Opțional: Resetează cheia primară
    const std::string resetAutoincrementSQL = "DELETE FROM sqlite_sequence WHERE name='User';";
    rc = sqlite3_exec(db, resetAutoincrementSQL.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error while resetting autoincrement: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "Autoincrement reset successfully!" << std::endl;
    }
}
int UserDatabase::getUserIdByUsername(const std::string& username)
{
    const std::string querySQL = "SELECT id FROM User WHERE username = ?;";
    sqlite3_stmt* stmt;

    // Pregătește interogarea SQL
    int rc = sqlite3_prepare_v2(db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return -1; // Returnează -1 în caz de eroare
    }

    // Bind pentru username
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    // Execută interogarea și returnează ID-ul dacă este găsit
    int userId = -1; // Default: -1 indică faptul că utilizatorul nu a fost găsit
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0); // Obține ID-ul din prima coloană
    }
    else {
        std::cerr << "Username not found: " << username << std::endl;
    }

    // Finalizează interogarea
    sqlite3_finalize(stmt);

    return userId;
}

bool UserDatabase::authenticateUser(const std::string& username, const std::string& password) {
    const std::string query = "SELECT password FROM User WHERE username = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Asociază parametrii
    if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind username: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Execută interogarea
    bool isAuthenticated = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* storedPassword = sqlite3_column_text(stmt, 0);
        if (storedPassword && password == reinterpret_cast<const char*>(storedPassword)) {
            isAuthenticated = true;
        }
    }

    sqlite3_finalize(stmt);

    if (!isAuthenticated) {
        std::cerr << "Authentication failed for user: " << username << std::endl;
    }

    return isAuthenticated;
}
