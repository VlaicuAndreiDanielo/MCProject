#pragma once
#include <string>
#include <vector>
class Username
{
	static std::vector<std::string> takenUsernames;
    Username(const std::string& username, const std::string& password);

    std::string getUsername() const;
    std::string getPassword() const;

    bool isUsernameValid() const;
    bool isPasswordValid() const;

    bool setUsername(const std::string& newUsername);
    bool setPassword(const std::string& newPassword);
private:
	std::string username;
	std::string password;
	bool validUsername;
	bool validPassword;

	bool isValidUsername(const std::string& name);
	bool isValidPassword(const std::string& password);
};

