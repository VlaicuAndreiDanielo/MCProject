#include "UserDatabase.h"

UserDatabase::UserDatabase(const std::string& dbName)
{
    int result = sqlite3_open(dbName.c_str(), &m_db);

    if (result != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << std::endl;
        m_db = nullptr;
    }
    else {
        std::cout << "Database opened successfully: " << dbName << std::endl;

        // Asigură-te că tabela este creată
        try {
            CreateTable();
        }
        catch (const std::exception& e) {
            std::cerr << "Exception while creating table: " << e.what() << std::endl;
            sqlite3_close(m_db); // Închide baza de date în caz de eroare
            m_db = nullptr;
        }
    }
}

UserDatabase::~UserDatabase()
{
    if (m_db) {
        sqlite3_close(m_db);
        std::cout << "Database closed!" << std::endl;
    }
}

void UserDatabase::CreateTable()
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

    ExecuteSQL(createTableSQL);
}

void UserDatabase::AddUser(const User& user)
{
    // Validare de bază pentru username și password
    if (!user.IsUsernameValid() || !user.IsPasswordValid()) {
        std::cerr << "Invalid user. Cannot add to database." << std::endl;
        return;
    }

    const std::string& username = user.GetUsername();
    const std::string& password = user.GetPassword();

    if (username.empty() || password.empty()) {
        std::cerr << "Error: Username or password cannot be empty!" << std::endl;
        return;
    }

    // Verifică dacă utilizatorul există deja
    if (UserExists(username)) {
        std::cerr << "Error: User with username '" << username << "' already exists in the database." << std::endl;
        return;
    }

    // Debugging: Afișează valorile utilizatorului
    std::cout << "Adding User - Username: " << user.GetUsername()
        << ", Password: " << user.GetPassword()
        << ", Score: " << user.GetScore()
        << ", Upgrade Points: " << user.GetUpgradePoints() << std::endl;

    const std::string insertSQL = "INSERT INTO User (username, password, score, upgradePoints) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    // Pregătirea interogării
    int rc = sqlite3_prepare_v2(m_db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    // Bind pentru valorile utilizatorului
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user.GetScore());
    sqlite3_bind_int(stmt, 4, user.GetUpgradePoints());

    // Execută interogarea
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(m_db) << std::endl;
        std::cerr << "Failed SQL: " << insertSQL << std::endl;
    }
    else {
        std::cout << "User added successfully!" << std::endl;
    }

    // Finalizează statement-ul
    sqlite3_finalize(stmt);
}



void UserDatabase::UpdateUserScore(const std::string& username, int newScore)
{
    const std::string updateSQL = "UPDATE User SET score = ? WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, updateSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, newScore);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void UserDatabase::UpdateUserUpgradePoints(const std::string& username, int newPoints)
{
    const std::string updateSQL = "UPDATE User SET upgradePoints = ? WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, updateSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, newPoints);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

User UserDatabase::GetUserInfo(const std::string& username)
{
    const std::string querySQL = "SELECT * FROM User WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query: " + std::string(sqlite3_errmsg(m_db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    User user("", "", 0, 0);  // Obiect User gol
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user.SetUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        user.SetPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        user.SetScore(sqlite3_column_int(stmt, 3));
        user.SetUpgradePoints(sqlite3_column_int(stmt, 4));
    }
    else {
        throw std::runtime_error("User not found!");
    }

    sqlite3_finalize(stmt);
    return user;
}

void UserDatabase::DeleteUser(const std::string& username)
{
    const std::string deleteSQL = "DELETE FROM User WHERE username = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, deleteSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void UserDatabase::ShowAllUsers()
{
    const std::string querySQL = "SELECT * FROM User;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query: " + std::string(sqlite3_errmsg(m_db)));
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


bool UserDatabase::UserExists(const std::string& username)
{
    const std::string querySQL = "SELECT COUNT(*) FROM User WHERE username = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
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


void UserDatabase::ExecuteSQL(const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << sql << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "SQL executed successfully: " << sql << std::endl;
    }
}

void UserDatabase::ClearTable()
{
    const std::string clearTableSQL = "DELETE FROM User;";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(m_db, clearTableSQL.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error while clearing table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "All rows deleted from the table successfully!" << std::endl;
    }

    // Opțional: Resetează cheia primară
    const std::string resetAutoincrementSQL = "DELETE FROM sqlite_sequence WHERE name='User';";
    rc = sqlite3_exec(m_db, resetAutoincrementSQL.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error while resetting autoincrement: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "Autoincrement reset successfully!" << std::endl;
    }
}
int UserDatabase::GetUserIdByUsername(const std::string& username)
{
    const std::string querySQL = "SELECT id FROM User WHERE username = ?;";
    sqlite3_stmt* stmt;

    // Pregătește interogarea SQL
    int rc = sqlite3_prepare_v2(m_db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
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

bool UserDatabase::AuthenticateUser(const std::string& username, const std::string& password) {
    const std::string query = "SELECT password FROM User WHERE username = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    // Asociază parametrii
    if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind username: " << sqlite3_errmsg(m_db) << std::endl;
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

std::string UserDatabase::GetUsernameById(int userId) {
    const std::string querySQL = "SELECT username FROM User WHERE id = ?;";
    sqlite3_stmt* stmt;

    // Pregătirea interogării SQL
    int rc = sqlite3_prepare_v2(m_db, querySQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return ""; // Returnează un string gol în caz de eroare
    }

    // Asociază parametrii (id-ul utilizatorului)
    sqlite3_bind_int(stmt, 1, userId);

    // Execută interogarea și obține username-ul dacă există
    std::string username;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)); // Extrage username-ul
    }
    else {
        std::cerr << "User ID not found: " << userId << std::endl;
        username = ""; // Returnează un string gol dacă ID-ul nu este găsit
    }

    // Finalizează statement-ul
    sqlite3_finalize(stmt);

    return username; // Returnează username-ul găsit
}
