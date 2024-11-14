#include "username.h"
#include <regex>
#include <algorithm>
#include <ctime>

std::vector<std::string> Username::takenUsernames;
std::vector<std::string> Username::userPasswords;

Username::Username(const std::string& username, const std::string& password) {
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
}

std::string Username::getUsername() const {
    return username;
}

std::string Username::getPassword() const {
    return password;
}

bool Username::isUsernameValid() const {
    return validUsername;
}

bool Username::isPasswordValid() const {
    return validPassword;
}

bool Username::setUsername(const std::string& newUsername) {
    if (isValidUsername(newUsername) && std::find(takenUsernames.begin(), takenUsernames.end(), newUsername) == takenUsernames.end()) {
        username = newUsername;
        takenUsernames.push_back(newUsername);  
        return true;
    }
    return false;
}

bool Username::setPassword(const std::string& newPassword) {
    if (isValidPassword(newPassword) && checkPasswordStrength(newPassword)) {
        password = newPassword;
        return true;
    }
    return false;
}

std::time_t Username::getAccountCreationTime() const {
    return accountCreated;
}

bool Username::authenticate(const std::string& inputPassword) {
   
    auto it = std::find(takenUsernames.begin(), takenUsernames.end(), username);
    if (it != takenUsernames.end()) {
        size_t index = std::distance(takenUsernames.begin(), it);
        return userPasswords[index] == inputPassword;
    }
    return false;
}

bool Username::resetPassword(const std::string& oldPassword, const std::string& newPassword) {
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

bool Username::checkPasswordStrength(const std::string& password) const {
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

bool Username::isValidUsername(const std::string& name) {
    std::regex pattern("^[a-zA-Z0-9_-.]{5,}$");
    return std::regex_match(name, pattern);
}

bool Username::isValidPassword(const std::string& password) {
    std::regex pattern("^.{8,}$");
    return std::regex_match(password, pattern);
}
