#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Creează widgetul central și layout-ul
    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    // Creează butoanele Login și Sign In
    loginButton = new QPushButton("Login", this);
    signInButton = new QPushButton("Sign In", this);

    // Adaugă butoanele în layout
    layout->addWidget(loginButton);
    layout->addWidget(signInButton);

    // Setează layout-ul principal
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Conectează butoanele la sloturi
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::showLoginForm);
    connect(signInButton, &QPushButton::clicked, this, &MainWindow::showSignInForm);
}

void MainWindow::clearLayout() {
    // Șterge toate widgeturile din layout
    while (layout->count() > 0) {
        QWidget* widget = layout->takeAt(0)->widget();
        if (widget) {
            widget->hide();
            delete widget;
        }
    }
}

void MainWindow::showLoginForm() {
    // Curăță layout-ul
    clearLayout();

    // Creează câmpurile pentru login
    usernameLabel = new QLabel("Username:", this);
    usernameField = new QLineEdit(this);

    passwordLabel = new QLabel("Password:", this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password);

    submitButton = new QPushButton("Submit", this);

    // Adaugă câmpurile în layout
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameField);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordField);
    layout->addWidget(submitButton);
}

void MainWindow::showSignInForm() {
    // Curăță layout-ul
    clearLayout();

    // Creează câmpurile pentru sign in
    usernameLabel = new QLabel("Username:", this);
    usernameField = new QLineEdit(this);

    passwordLabel = new QLabel("Password:", this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password);

    confirmPasswordLabel = new QLabel("Confirm Password:", this);
    confirmPasswordField = new QLineEdit(this);
    confirmPasswordField->setEchoMode(QLineEdit::Password);

    submitButton = new QPushButton("Submit", this);

    // Adaugă câmpurile în layout
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameField);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordField);
    layout->addWidget(confirmPasswordLabel);
    layout->addWidget(confirmPasswordField);
    layout->addWidget(submitButton);
}
