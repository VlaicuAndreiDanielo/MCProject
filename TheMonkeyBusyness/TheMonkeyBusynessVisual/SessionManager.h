#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager {
public:
    static QString GetCurrentUsername() {
        return currentUsername;
    }

    static void SetCurrentUsername(const QString& username) {
        currentUsername = username;
    }

private:
    static QString currentUsername; // Variabilă statică pentru username
};

#endif // SESSIONMANAGER_H
