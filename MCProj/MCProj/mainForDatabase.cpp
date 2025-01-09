#include "UserDatabase.h"
#include "User.h"

int main() {
    UserDatabase userDB("users.db");

    // Creează tabelă
    //userDB.createTable();

    // Creează utilizatori
    User user1("john_doe", "password123", 100, 50);  // Creează un obiect User
    User user2("jane_doe", "mypassword", 200, 70);   // Creează alt obiect User

    // Adaugă utilizatori
    userDB.AddUser(user1);  // Trimite obiectul User
    userDB.AddUser(user2);  // Trimite obiectul User

    // Actualizează scorul unui utilizator
    //userDB.updateUserScore("john_doe", 150);

    // Afișează informațiile despre utilizatori
    //userDB.getUserInfo("john_doe");
    std::cout << "Id-ul user-ului este: " << userDB.GetUserIdByUsername(user1.GetUsername()) << "\n";
    //userDB.getUserInfo("jane_doe");
    //
    userDB.ShowAllUsers();

    // Șterge un utilizator
    //userDB.deleteUser("passward");
    //userDB.deleteUser("mypasswo");
    // userDB.clearTable();
    return 0;
}
