#include "SignInForm.h"
#include "PlayWindow.h"
#include "UserDatabase.h"


SignInForm::SignInForm(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Sign In");
    setFixedSize(700, 700); // Setează dimensiunea fixă pentru fereastră

    // **Setează imaginea ca fundal**
    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background.jpg";
    QPixmap background(imagePath);
    if (!background.isNull()) {
        background = background.scaled(size(), Qt::KeepAspectRatioByExpanding);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(background));
        setPalette(palette);
    }

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Creează titlul
    QLabel* titleLabel = new QLabel("Sign In", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: rgba(255, 223, 0, 255);");
    titleLabel->setAlignment(Qt::AlignCenter);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    titleLabel->setGraphicsEffect(shadowEffect);

    layout->addWidget(titleLabel);

    m_usernameLabel = new QLabel("Username:", this);
    m_usernameField = new QLineEdit(this);
    m_usernameField->setPlaceholderText("Enter your username"); // Adăugare placeholder

    m_passwordLabel = new QLabel("Password:", this);
    m_passwordField = new QLineEdit(this);
    m_passwordField->setEchoMode(QLineEdit::Password);
    m_passwordField->setPlaceholderText("Enter your password"); // Adăugare placeholder

    m_confirmPasswordLabel = new QLabel("Confirm Password:", this);
    m_confirmPasswordField = new QLineEdit(this);
    m_confirmPasswordField->setEchoMode(QLineEdit::Password);
    m_confirmPasswordField->setPlaceholderText("Confirm your password"); // Adăugare placeholder

    m_submitButton = new QPushButton("Submit", this);
    m_backButton = new QPushButton("Back", this);

    // Stilizare pentru câmpuri și butoane
    QString fieldStyle = R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 200);
            color: black;
            border: 2px solid black;
            border-radius: 10px;
            padding: 5px;
            font-size: 16px;
        }
        QPushButton {
            background-color: rgba(255, 255, 0, 200);
            color: black;
            font-size: 18px;
            font-weight: bold;
            border-radius: 10px;
            padding: 10px;
        }
        QPushButton:hover {
            background-color: rgba(255, 215, 0, 230);
        }
    )";
    m_usernameField->setStyleSheet(fieldStyle);
    m_passwordField->setStyleSheet(fieldStyle);
    m_confirmPasswordField->setStyleSheet(fieldStyle);
    m_submitButton->setStyleSheet(fieldStyle);
    m_backButton->setStyleSheet(fieldStyle);

    QString labelStyle = R"(
        QLabel {
            color: rgba(0, 0, 0, 255);
            font-size: 18px;
            font-weight: bold;
        }
    )";
    m_usernameLabel->setStyleSheet(labelStyle);
    m_passwordLabel->setStyleSheet(labelStyle);
    m_confirmPasswordLabel->setStyleSheet(labelStyle);

    m_usernameField->setFixedSize(300, 40);
    m_passwordField->setFixedSize(300, 40);
    m_confirmPasswordField->setFixedSize(300, 40);
    m_submitButton->setFixedSize(200, 50);
    m_backButton->setFixedSize(200, 50);

    layout->addWidget(m_usernameLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_usernameField, 0, Qt::AlignCenter);
    layout->addWidget(m_passwordLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_passwordField, 0, Qt::AlignCenter);
    layout->addWidget(m_confirmPasswordLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_confirmPasswordField, 0, Qt::AlignCenter);
    layout->addWidget(m_submitButton, 0, Qt::AlignCenter);
    layout->addWidget(m_backButton, 0, Qt::AlignCenter);

    connect(m_backButton, &QPushButton::clicked, this, &SignInForm::backRequested); // Emit semnalul backRequested
    // Conectează butonul Submit
    /*connect(m_submitButton, &QPushButton::clicked, [=]() {
    QString username = m_usernameField->text(); // Citește utilizatorul
    QString password = m_passwordField->text(); // Citește parola
    QString confirmPassword = m_confirmPasswordField->text(); // Confirmă parola

    UserDatabase db("userdatabase.db");
    db.createTable(); // Asigură-te că tabela există

    if (!m_usernameField || !m_passwordField || !m_confirmPasswordField) {
        QMessageBox::critical(this, "Error", "Input fields are not initialized.");
        return;
    }

    // Regex pentru validare utilizator (doar litere și cifre, între 3 și 15 caractere)
    QRegularExpression usernameRegex("^[a-zA-Z0-9]{3,15}$");
    if (!usernameRegex.match(username).hasMatch()) {
        QMessageBox::warning(this, "Validation Error", "Username must be 3-15 characters long and contain only letters and numbers.");
        return;
    }

    // Regex pentru validare parolă (cel puțin 8 caractere, litere mari/mici, cifre, simboluri)
    QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$");
    if (!passwordRegex.match(password).hasMatch()) {
        QMessageBox::warning(this, "Validation Error", "Password must be at least 8 characters long, include uppercase, lowercase, a digit, and a special character.");
        return;
    }

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username or password cannot be empty!");
        return;
    }

    // Verifică dacă numele de utilizator există deja
    if (db.userExists(username.toStdString())) {
        QMessageBox::warning(this, "Sign Up Failed", "Username already exists.");
        return;
    }

    // Verifică dacă parolele coincid
    if (password != confirmPassword) {
        QMessageBox::warning(this, "Sign Up Failed", "Passwords do not match.");
        return;
    }

    // Creează utilizatorul și adaugă-l în baza de date
    User newUser(username.toStdString(), password.toStdString());
    db.addUser(newUser);

    QMessageBox::information(this, "Sign Up Successful", "Account created successfully!");
    close(); // Închide SignInForm
});

    */

    disconnect(m_submitButton, nullptr, nullptr, nullptr);
    //Varinata in care se vede varianta functionala
    /*connect(m_submitButton, &QPushButton::clicked, [=]() {

        qDebug() << "m_usernameField created: " << (m_usernameField != nullptr);
        qDebug() << "m_passwordField created: " << (m_passwordField != nullptr);
        qDebug() << "m_confirmPasswordField created: " << (m_confirmPasswordField != nullptr);

        qDebug() << "m_usernameField parent:" << m_usernameField->parent();
        qDebug() << "m_passwordField parent:" << m_passwordField->parent();
        qDebug() << "m_confirmPasswordField parent:" << m_confirmPasswordField->parent();


        if (m_usernameField->text().isEmpty()) {
            qDebug() << "Username field is empty!";
            return;
        }
        if (m_passwordField->text().isEmpty()) {
            qDebug() << "Password field is empty!";
            return;
        }
        if (m_confirmPasswordField->text().isEmpty()) {
            qDebug() << "Confirm Password field is empty!";
            return;
        }

        QString username = m_usernameField->text(); //m_usernameField->displayText();
        QString password = m_passwordField->text(); //m_passwordField->displayText();
        QString confirmPassword = m_confirmPasswordField->text(); //m_confirmPasswordField->displayText();

        qDebug() << "Username: " << username;
        qDebug() << "Password: " << password;
        qDebug() << "ConfirmPassword: " << confirmPassword;

        std::string usernameStd = username.toUtf8().constData(); // Conversia aceasta este functionala
        std::string passwordStd = password.toUtf8().constData(); // Conversia aceasta este functionala
        std::string confirmPasswordStd = confirmPassword.toUtf8().constData(); // Conversia aceasta este functionala

        qDebug() << "Username as std::string:" << QString::fromStdString(usernameStd); //Duce la coruperea textului
        qDebug() << "Password as std::string:" << QString::fromStdString(passwordStd); //Duce la coruperea textului
        qDebug() << "Confirm Password as std::string:" << QString::fromStdString(confirmPasswordStd); //Duce la coruperea textului

        if (!m_usernameField) { std::cerr << "Error: m_usernameField is null!" << std::endl; return; }
        //QString username = m_usernameField->text();// Citește utilizatorul
        std::cout << "Username: " << username.toStdString() << std::endl; 
        if (!m_passwordField) { std::cerr << "Error: m_passwordField is null!" << std::endl; return; }
        //QString password = m_passwordField->text();// Citește parola
        std::cout << "Password: " << password.toStdString() << std::endl;
        if (!m_confirmPasswordField) { std::cerr << "Error: m_confirmPasswordField is null!" << std::endl; return; }
        //QString confirmPassword = m_confirmPasswordField->text(); // Confirmă parola
        std::cout << "ConfirmPassword: " << confirmPassword.toStdString() << std::endl;

        UserDatabase db("userdatabase.db");
        db.createTable();

        if (!m_usernameField || !m_passwordField || !m_confirmPasswordField) {
            QMessageBox::critical(this, "Error", "Input fields are not initialized.");
            return;
        }
        std::cout << "Username: " << username.toStdString() << ", Password: " << password.toStdString() << std::endl;
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Error", "Username or password cannot be empty!");
            return;
        }
        // Verificare folosind funcția din clasa User
        User tempUser(username.toStdString(), password.toStdString());
        if (!tempUser.isUsernameValid()) {
            QMessageBox::warning(this, "Validation Error", "Invalid username format.");
            return;
        }

        if (!tempUser.isPasswordValid()) {
            QMessageBox::warning(this, "Validation Error", "Invalid password format.");
            return;
        }

        // Verifică dacă numele de utilizator există deja
        if (db.userExists(username.toStdString())) {
            QMessageBox::warning(this, "Sign Up Failed", "Username already exists.");
            return;
        }

        // Verifică dacă parolele coincid
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Sign Up Failed", "Passwords do not match.");
            return;
        }

        // Creează utilizatorul și adaugă-l în baza de date
        User newUser(username.toStdString(), password.toStdString());
        db.addUser(newUser);

        QMessageBox::information(this, "Sign Up Successful", "Account created successfully!");
        close(); // Închide SignInForm
        });

}*/
    connect(m_submitButton, &QPushButton::clicked, [=]() {

        qDebug() << "m_usernameField created: " << (m_usernameField != nullptr);
        qDebug() << "m_passwordField created: " << (m_passwordField != nullptr);
        qDebug() << "m_confirmPasswordField created: " << (m_confirmPasswordField != nullptr);

        if (m_usernameField->text().isEmpty()) {
            qDebug() << "Username field is empty!";
            return;
        }
        if (m_passwordField->text().isEmpty()) {
            qDebug() << "Password field is empty!";
            return;
        }
        if (m_confirmPasswordField->text().isEmpty()) {
            qDebug() << "Confirm Password field is empty!";
            return;
        }

        QString username = m_usernameField->text(); //m_usernameField->displayText();
        QString password = m_passwordField->text(); //m_passwordField->displayText();
        QString confirmPassword = m_confirmPasswordField->text(); //m_confirmPasswordField->displayText();

        std::string usernameStd = username.toUtf8().constData(); // Conversia aceasta este functionala
        std::string passwordStd = password.toUtf8().constData(); // Conversia aceasta este functionala
        std::string confirmPasswordStd = confirmPassword.toUtf8().constData(); // Conversia aceasta este functionala

        std::cout << "Username: " << usernameStd << std::endl;
        std::cout << "Password: " << passwordStd << std::endl;
        std::cout << "ConfirmPassword: " << confirmPasswordStd << std::endl;

        UserDatabase db("userdatabase.db");
        db.createTable();

        if (!m_usernameField || !m_passwordField || !m_confirmPasswordField) {
            QMessageBox::critical(this, "Error", "Input fields are not initialized.");
            return;
        }
        std::cout << "Username: " << usernameStd << ", Password: " << passwordStd << std::endl;
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Error", "Username or password cannot be empty!");
            return;
        }
        // Verificare folosind funcția din clasa User
        User tempUser(usernameStd, passwordStd);
        if (!tempUser.isUsernameValid()) {
            QMessageBox::warning(this, "Validation Error", "Invalid username format.");
            return;
        }

        if (!tempUser.isPasswordValid()) {
            QMessageBox::warning(this, "Validation Error", "Invalid password format.");
            return;
        }

        // Verifică dacă numele de utilizator există deja
        if (db.userExists(usernameStd)) {
            QMessageBox::warning(this, "Sign Up Failed", "Username already exists.");
            return;
        }

        // Verifică dacă parolele coincid
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Sign Up Failed", "Passwords do not match.");
            return;
        }

        // Creează utilizatorul și adaugă-l în baza de date
        User newUser(usernameStd, passwordStd);
        db.addUser(newUser);

        QMessageBox::information(this, "Sign Up Successful", "Account created successfully!");
        close(); // Închide SignInForm
        });

}