#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager {
public:
    static QString getCurrentUsername() {
        return currentUsername;
    }

    static void setCurrentUsername(const QString& username) {
        currentUsername = username;
    }

private:
    static QString currentUsername; // Variabilă statică pentru username
};

#endif // SESSIONMANAGER_H
