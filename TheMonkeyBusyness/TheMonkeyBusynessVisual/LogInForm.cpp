#include "LoginForm.h"
#include "PlayWindow.h"
#include "UserDatabase.h"
#include "SessionManager.h"


LoginForm::LoginForm(QWidget* parent) : QDialog(parent) {
    setFixedSize(700, 700); // Setează dimensiunea fixă pentru fereastră
    setWindowTitle("Login");

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
    QLabel* titleLabel = new QLabel("Login", this);
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

    m_usernameField->setFixedSize(300, 40);
    m_passwordField->setFixedSize(300, 40);
    m_submitButton->setFixedSize(200, 50);
    m_backButton->setFixedSize(200, 50);

    layout->addWidget(m_usernameLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_usernameField, 0, Qt::AlignCenter);
    layout->addWidget(m_passwordLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_passwordField, 0, Qt::AlignCenter);
    layout->addWidget(m_submitButton, 0, Qt::AlignCenter);
    layout->addWidget(m_backButton, 0, Qt::AlignCenter);

    connect(m_backButton, &QPushButton::clicked, this, &LoginForm::BackRequested); // Emit semnalul backRequested
    // Conectează butonul Submit
    connect(m_submitButton, &QPushButton::clicked, [=]() {
        QString username = m_usernameField->text();
        QString password = m_passwordField->text();

        std::string usernameStd = username.toUtf8().constData(); // Conversia aceasta este functionala
        std::cout << "Username: " << usernameStd << std::endl;
        std::string passwordStd = password.toUtf8().constData(); // Conversia aceasta este functionala
        std::cout << "Password: " << passwordStd << std::endl;


        UserDatabase m_db("userdatabase.db");
        if (!m_usernameField || !m_passwordField) {
            QMessageBox::critical(this, "Error", "Input fields are not initialized properly!");
            return;
        }
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Login Failed", "Username or password cannot be empty!");
            return;
        }

        // Verifică dacă utilizatorul există în baza de date
        if (!m_db.UserExists(usernameStd)) {
            QMessageBox::warning(this, "Login Failed", "Username does not exist.");
            return;
        }

        // Verifică dacă parola este corectă
        if (!m_db.AuthenticateUser(usernameStd, passwordStd)) {
            QMessageBox::warning(this, "Login Failed", "Invalid password.");
            return;
        }
        else
        {
            QMessageBox::information(this, "Login Successful", "Welcome back!");

            // Setează username-ul în SessionManager
            SessionManager::SetCurrentUsername(username);

            // Emit semnalul
            emit SessionStarted();

            // Deschide PlayWindow după logare reușită
            PlayWindow* playWindow = new PlayWindow();
            playWindow->setAttribute(Qt::WA_DeleteOnClose); // Eliberare memorie la închidere
            playWindow->show();
            m_db.ShowAllUsers();
            close(); // Închide LogInForm
        }

        });

}