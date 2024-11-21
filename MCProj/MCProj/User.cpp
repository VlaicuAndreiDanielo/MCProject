#include "User.h"
#include <regex>
#include <algorithm>
#include <ctime>

std::vector<std::string> User::takenUsernames;
std::vector<std::string> User::userPasswords;

User::User(const std::string& username, const std::string& password) {
    this->validUsername = isValidUsername(username);
    this->validPassword = isValidPassword(password);

    if (validUsername && validPassword && std::find(takenUsernames.begin(), takenUsernames.end(), username) == takenUsernames.end()) {
        this->username = username;
        this->password = password;  
        takenUsernames.push_back(username);
        userPasswords.push_back(password);  
        this->accountCreated = std::time(0);
    }
    else {
        this->validUsername = false;
        this->validPassword = false;
    }
    score = 0;
    upgradePoints = 0;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

bool User::isUsernameValid() const {
    return validUsername;
}

bool User::isPasswordValid() const {
    return validPassword;
}

bool User::setUsername(const std::string& newUsername) {
    if (isValidUsername(newUsername) && std::find(takenUsernames.begin(), takenUsernames.end(), newUsername) == takenUsernames.end()) {
        username = newUsername;
        takenUsernames.push_back(newUsername);  
        return true;
    }
    return false;
}

bool User::setPassword(const std::string& newPassword) {
    if (isValidPassword(newPassword) && checkPasswordStrength(newPassword)) {
        password = newPassword;
        return true;
    }
    return false;
}

std::time_t User::getAccountCreationTime() const {
    return accountCreated;
}

bool User::authenticate(const std::string& inputPassword) {
   
    auto it = std::find(takenUsernames.begin(), takenUsernames.end(), username);
    if (it != takenUsernames.end()) {
        size_t index = std::distance(takenUsernames.begin(), it);
        return userPasswords[index] == inputPassword;
    }
    return false;
}

bool User::resetPassword(const std::string& oldPassword, const std::string& newPassword) {
    if (authenticate(oldPassword) && isValidPassword(newPassword)) {
        password = newPassword;
        
        auto it = std::find(takenUsernames.begin(), takenUsernames.end(), username);
        if (it != takenUsernames.end()) {
            size_t index = std::distance(takenUsernames.begin(), it);
            userPasswords[index] = newPassword;
        }
        return true;
    }
    return false;
}

bool User::checkPasswordStrength(const std::string& password) const {
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    std::regex specialChars("[!@#$%^&*(),.?\":{}|<>]");

    for (char ch : password) {
        if (std::isupper(ch)) hasUpper = true;
        if (std::islower(ch)) hasLower = true;
        if (std::isdigit(ch)) hasDigit = true;
        if (std::regex_match(std::string(1, ch), specialChars)) hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

int User::getScore() const
{
    return score;
}

int User::getUpgradePoints() const
{
    return upgradePoints;
}

void User::setScore(const int &scr)
{
    score = scr;
}

void User::setUpgradePoints(const int& upt)
{
    upgradePoints = upt;
}

bool User::isValidUsername(const std::string& name) {
    std::regex pattern("^[a-zA-Z0-9_-.]{5,}$");
    return std::regex_match(name, pattern);
}

bool User::isValidPassword(const std::string& password) {
    std::regex pattern("^.{8,}$");
    return std::regex_match(password, pattern);
}
