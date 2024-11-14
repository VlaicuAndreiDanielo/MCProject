#include "username.h"
#include <regex>
#include <algorithm>
std::vector<std::string> Username::takenUsernames;

Username::Username(const std::string& username, const std::string& password) {
    this->validUsername = isValidUsername(username);
    this->validPassword = isValidPassword(password);

    if (validUsername && validPassword && std::find(takenUsernames.begin(), takenUsernames.end(), username) == takenUsernames.end()) {
        this->username = username;
        this->password = password;
        takenUsernames.push_back(username);  
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
    if (isValidPassword(newPassword)) {
        password = newPassword;
        return true;
    }
    return false;
}

bool Username::isValidUsername(const std::string& name) {
    std::regex pattern("^[a-zA-Z0-9_-.]{5,}$");
    return std::regex_match(name, pattern);
}


bool Username::isValidPassword(const std::string& password) {
    std::regex pattern("^[a-zA-Z0-9_-.$%^&*#!~+=<>?:]{8,}$");
    return std::regex_match(password, pattern);
}