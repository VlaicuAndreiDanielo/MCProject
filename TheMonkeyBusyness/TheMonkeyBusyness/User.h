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
    User(const std::string& username, const std::string& password);
    int GetUserId() const;
    std::string GetUsername() const;
    std::string GetPassword() const;
    bool IsUsernameValid() const;
    bool IsPasswordValid() const;
    bool SetUsername(const std::string& newUsername);
    bool SetPassword(const std::string& newPassword);
    std::time_t GetAccountCreationTime() const;
    bool Authenticate(const std::string& inputPassword);
    bool ResetPassword(const std::string& oldPassword, const std::string& newPassword);

    static bool IsValidUsername(const std::string& name);
    static bool IsValidPassword(const std::string& password);
    bool CheckPasswordStrength(const std::string& password) const;

    int GetScore() const;
    int GetUpgradePoints() const;

    void SetScore(int scr);
    void SetUpgradePoints(int upt);
};


