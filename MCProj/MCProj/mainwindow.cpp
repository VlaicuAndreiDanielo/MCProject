#include "mainwindow.h"


#include "mainwindow.h"

//MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
//    // Creează widgetul central și layout-ul
//    m_centralWidget = new QWidget(this);
//    m_layout = new QVBoxLayout(m_centralWidget);
//
//    // Setează distanța dintre butoane
//    m_layout->setSpacing(1); // Reduce spacing-ul la 10 pixeli (modifică după preferințe)
//
//    // Creează butoanele Login și Sign In
//    m_loginButton = new QPushButton("Login", this);
//    m_signInButton = new QPushButton("Sign In", this);
//
//    // Setează stilul pentru butoane
//    QString buttonStyle = R"(
//        QPushButton {
//            background-color: rgba(255, 255, 0, 200); /* Galben transparent */
//            color: black; /* Culoarea textului */
//            font-size: 18px; /* Dimensiunea fontului */
//            font-weight: bold; /* Text îngroșat */
//            border-radius: 5px; /* Colțuri rotunjite */
//            padding: 5px; /* Spațiere internă */
//        }
//        QPushButton:hover {
//            background-color: rgba(255, 215, 0, 230); /* Culoare mai închisă la hover */
//        }
//    )";
//    m_loginButton->setStyleSheet(buttonStyle);
//    m_signInButton->setStyleSheet(buttonStyle);
//
//    // Setează dimensiunea butoanelor
//    m_loginButton->setFixedSize(200, 50); // Lățime 200px, Înălțime 50px
//    m_signInButton->setFixedSize(200, 50);
//
//    // Adaugă butoanele în layout
//    m_layout->addWidget(m_loginButton);
//    m_layout->addWidget(m_signInButton);
//
//    // Centrează layout-ul
//    m_layout->setAlignment(m_loginButton, Qt::AlignCenter);
//    m_layout->setAlignment(m_signInButton, Qt::AlignCenter);
//
//    // Setează layout-ul principal
//    m_centralWidget->setLayout(m_layout);
//    setCentralWidget(m_centralWidget);
//
//    // **Setează imaginea ca fundal**
//    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background.jpg";
//    QPixmap background(imagePath);
//    if (background.isNull()) {
//        qDebug() << "Imaginea nu a fost încărcată corect!";
//    }
//    else {
//        qDebug() << "Imaginea a fost încărcată cu succes!";
//    }
//    background = background.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(background));
//    setPalette(palette);
//
//    // Dimensionează fereastra conform imaginii
//    resize(background.size());
//
//    // Conectează butoanele la sloturi
//    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::ShowLoginForm);
//    connect(m_signInButton, &QPushButton::clicked, this, &MainWindow::ShowSignInForm);
//}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Creează widgetul central și layout-ul
    m_centralWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centralWidget);

    // Setează distanța dintre butoane
    m_layout->setContentsMargins(0, 30, 0, 30); // Margini sus-jos
    m_layout->setSpacing(10); // Spațiu între widgeturi

    // Creează un QLabel pentru titlu
    QLabel* titleLabel = new QLabel("Monkeys Madness", this);
    QFont titleFont("Arial", 36, QFont::Bold); // Font mare și bold
    titleLabel->setFont(titleFont);

    // Setează culoarea textului
    titleLabel->setStyleSheet("color: rgba(255, 223, 0, 255);");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Creează efectul de umbră
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(2, 2); // Deplasare a umbrei
    shadowEffect->setBlurRadius(10); // Grad de blur pentru umbră
    shadowEffect->setColor(QColor(0, 0, 0, 150)); // Culoare neagră semi-transparentă
    titleLabel->setGraphicsEffect(shadowEffect);

    // Adaugă titlul la layout
    m_layout->addWidget(titleLabel);
    m_layout->setAlignment(titleLabel, Qt::AlignCenter);

    // Creează butoanele Login și Sign In
    m_loginButton = new QPushButton("Login", this);
    m_signInButton = new QPushButton("Sign In", this);

    // Stilizează butoanele
    QString buttonStyle = R"(
        QPushButton {
            background-color: rgba(255, 255, 0, 200); /* Galben transparent */
            color: black; /* Culoarea textului */
            font-size: 18px; /* Dimensiunea fontului */
            font-weight: bold; /* Text îngroșat */
            border-radius: 5px; /* Colțuri rotunjite */
            padding: 5px; /* Spațiere internă */
        }
        QPushButton:hover {
            background-color: rgba(255, 215, 0, 230); /* Culoare mai închisă la hover */
        }
    )";
    m_loginButton->setStyleSheet(buttonStyle);
    m_signInButton->setStyleSheet(buttonStyle);

    // Setează dimensiunile butoanelor
    m_loginButton->setFixedSize(200, 50);
    m_signInButton->setFixedSize(200, 50);

    // Adaugă butoanele în layout
    m_layout->addWidget(m_loginButton);
    m_layout->addWidget(m_signInButton);

    // Centrează layout-ul
    m_layout->setAlignment(m_loginButton, Qt::AlignCenter);
    m_layout->setAlignment(m_signInButton, Qt::AlignCenter);

    // Setează layout-ul principal
    m_centralWidget->setLayout(m_layout);
    setCentralWidget(m_centralWidget);

    // **Setează imaginea ca fundal**
    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background.jpg";
    QPixmap background(imagePath);
    if (background.isNull()) {
        qDebug() << "Imaginea nu a fost încărcată corect!";
    }
    else {
        qDebug() << "Imaginea a fost încărcată cu succes!";
    }
    background = background.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
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
