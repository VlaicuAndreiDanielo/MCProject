#pragma once
#include <string>
#include <vector>
#include <ctime>

class User
{
    static std::vector<std::string> takenUsernames;  // Lista de nume de utilizatori deja folosite
    static std::vector<std::string> userPasswords;   // Lista de parole pentru utilizatori

private:
    std::string username;
    std::string password;
    bool validUsername;
    bool validPassword;
    std::time_t accountCreated;
    int score;
    int upgradePoints;

public:
    User(const std::string& username, const std::string& password);

    std::string getUsername() const;
    std::string getPassword() const;
    bool isUsernameValid() const;
    bool isPasswordValid() const;
    bool setUsername(const std::string& newUsername);
    bool setPassword(const std::string& newPassword);
    std::time_t getAccountCreationTime() const;
    bool authenticate(const std::string& inputPassword); 
    bool resetPassword(const std::string& oldPassword, const std::string& newPassword);

    static bool isValidUsername(const std::string& name);  
    static bool isValidPassword(const std::string& password);  
    bool checkPasswordStrength(const std::string& password) const;  

    int getScore() const;
    int getUpgratePoints() const;

    void setScore();
    void setUpgradePoints();
};


