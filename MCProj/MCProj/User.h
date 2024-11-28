#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <iostream>


class User
{
    static std::vector<std::string> takenUsernames;  // Lista de nume de utilizatori deja folosite
    static std::vector<std::string> userPasswords;   // Lista de parole pentru utilizatori

private:
    int m_userID;
    std::string m_username;
    std::string m_password;
    bool m_validUsername;
    bool m_validPassword;
    std::time_t m_accountCreated;
    int m_score;
    int m_upgradePoints;

public:
    User(const std::string& username, const std::string& password, int scr, int uppt);
    int getUserId() const;
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
    int getUpgradePoints() const;

    void setScore(int scr);
    void setUpgradePoints(int upt);
};


