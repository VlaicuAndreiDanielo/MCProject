#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Creează widgetul central și layout-ul
    m_centralWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centralWidget);

    // Creează butoanele Login și Sign In
    m_loginButton = new QPushButton("Login", this);
    m_signInButton = new QPushButton("Sign In", this);

    // Adaugă butoanele în layout
    m_layout->addWidget(m_loginButton);
    m_layout->addWidget(m_signInButton);

    // Setează layout-ul principal
    m_centralWidget->setLayout(m_layout);
    setCentralWidget(m_centralWidget);

    // **Setează imaginea ca fundal**
    QString imagePath = QCoreApplication::applicationDirPath() + "./Images/background.jpg";
    QPixmap background(imagePath); // Înlocuiește cu calea imaginii tale
    if (background.isNull()) {
        qDebug() << "Imaginea nu a fost încărcată corect!";
    }
    else {
        qDebug() << "Imaginea a fost încărcată cu succes!";
    }
    background = background.scaled(this->size(), Qt::KeepAspectRatioByExpanding); // Scalare automată
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(background));
    setPalette(palette);

    // Dimensionează fereastra conform imaginii
    resize(background.size());

    // Conectează butoanele la sloturi
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::ShowLoginForm);
    connect(m_signInButton, &QPushButton::clicked, this, &MainWindow::ShowSignInForm);
}


void MainWindow::ClearLayout() {
    // Șterge toate widgeturile din layout
    while (m_layout->count() > 0) {
        QWidget* widget = m_layout->takeAt(0)->widget();
        if (widget) {
            widget->hide();
            delete widget;
        }
    }
}

void MainWindow::ShowLoginForm() {
    // Curăță layout-ul
    ClearLayout();

    // Creează câmpurile pentru login
    m_usernameLabel = new QLabel("Username:", this);
    m_usernameField = new QLineEdit(this);

    m_passwordLabel = new QLabel("Password:", this);
    m_passwordField = new QLineEdit(this);
    m_passwordField->setEchoMode(QLineEdit::Password);

    m_submitButton = new QPushButton("Submit", this);

    // Adaugă câmpurile în layout
    m_layout->addWidget(m_usernameLabel);
    m_layout->addWidget(m_usernameField);
    m_layout->addWidget(m_passwordLabel);
    m_layout->addWidget(m_passwordField);
    m_layout->addWidget(m_submitButton);
}

void MainWindow::ShowSignInForm() {
    // Curăță layout-ul
    ClearLayout();

    // Creează câmpurile pentru sign in
    m_usernameLabel = new QLabel("Username:", this);
    m_usernameField = new QLineEdit(this);

    m_passwordLabel = new QLabel("Password:", this);
    m_passwordField = new QLineEdit(this);
    m_passwordField->setEchoMode(QLineEdit::Password);

    m_confirmPasswordLabel = new QLabel("Confirm Password:", this);
    m_confirmPasswordField = new QLineEdit(this);
    m_confirmPasswordField->setEchoMode(QLineEdit::Password);

    m_submitButton = new QPushButton("Submit", this);

    // Adaugă câmpurile în layout
    m_layout->addWidget(m_usernameLabel);
    m_layout->addWidget(m_usernameField);
    m_layout->addWidget(m_passwordLabel);
    m_layout->addWidget(m_passwordField);
    m_layout->addWidget(m_confirmPasswordLabel);
    m_layout->addWidget(m_confirmPasswordField);
    m_layout->addWidget(m_submitButton);
}
