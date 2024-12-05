#include "mainwindow.h"

//// Creează efectul de umbră
//QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//shadowEffect->setOffset(2, 2); // Deplasare a umbrei
//shadowEffect->setBlurRadius(10); // Grad de blur pentru umbră
//shadowEffect->setColor(QColor(0, 0, 0, 150)); // Culoare neagră semi-transparentă
//titleLabel->setGraphicsEffect(shadowEffect);

/*Varianta functionala*/
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
/*Verianta cu transparenta dar imi redimensioneaza imaginea*/
//MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
//    // Creează widgetul central și layout-ul
//    m_centralWidget = new QWidget(this);
//    m_layout = new QVBoxLayout(m_centralWidget);
//
//    // Setează distanța dintre butoane
//    m_layout->setContentsMargins(0, 30, 0, 30); // Margini sus-jos
//    m_layout->setSpacing(10); // Spațiu între widgeturi
//
//    // Creează un QLabel pentru titlu
//    QLabel* titleLabel = new QLabel("Monkeys Madness", this);
//    QFont titleFont("Arial", 36, QFont::Bold); // Font mare și bold
//    titleLabel->setFont(titleFont);
//
//    // Setează culoarea textului
//    titleLabel->setStyleSheet("color: rgba(255, 223, 0, 255);");
//    titleLabel->setAlignment(Qt::AlignCenter);
//
//    
//
//    // Adaugă titlul la layout
//    m_layout->addWidget(titleLabel);
//    m_layout->setAlignment(titleLabel, Qt::AlignCenter);
//
//    // Creează butoanele Login și Sign In
//    m_loginButton = new QPushButton("Login", this);
//    m_signInButton = new QPushButton("Sign In", this);
//
//    // Stilizează butoanele
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
//    // Setează dimensiunile butoanelor
//    m_loginButton->setFixedSize(200, 50);
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
//    // **Adaugă imaginea de fundal cu transparență**
//    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background.jpg";
//    QPixmap background(imagePath);
//    if (background.isNull()) {
//        qDebug() << "Imaginea nu a fost încărcată corect!";
//    }
//    else {
//        qDebug() << "Imaginea a fost încărcată cu succes!";
//    }
//
//    // Creează o versiune transparentă a imaginii
//    QPixmap transparentBackground(background.size());
//    transparentBackground.fill(Qt::transparent);
//
//    QPainter painter(&transparentBackground);
//    painter.setOpacity(0.6); // Nivelul de transparență (0.0 = complet transparent, 1.0 = complet opac)
//    painter.drawPixmap(0, 0, background);
//    painter.end();
//
//    // Creează un QLabel pentru imaginea de fundal
//    QLabel* backgroundLabel = new QLabel(this);
//    backgroundLabel->setPixmap(transparentBackground.scaled(this->size(), Qt::KeepAspectRatioByExpanding));
//    backgroundLabel->setGeometry(this->rect());
//    backgroundLabel->lower(); // Trimite QLabel în spate
//
//    // Dimensionează fereastra conform imaginii
//    resize(background.size());
//
//    // Conectează butoanele la sloturi
//    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::ShowLoginForm);
//    connect(m_signInButton, &QPushButton::clicked, this, &MainWindow::ShowSignInForm);
//}


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

    // Creează butonul „Back”
    m_backButton = new QPushButton("Back", this);
    QString backButtonStyle = R"(
        QPushButton {
            background-color: rgba(200, 200, 200, 200);
            color: black;
            font-size: 12px;
            border: 1px solid black;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: rgba(180, 180, 180, 200);
        }
    )";
    m_backButton->setStyleSheet(backButtonStyle);
    m_backButton->setFixedSize(50, 30);

    // Conectează butonul „Back” la metoda pentru ecranul principal
    connect(m_backButton, &QPushButton::clicked, this, &MainWindow::ShowMainScreen);

    // Creează un layout pentru butonul „Back” și poziționează-l în colțul stânga-sus
    QHBoxLayout* backLayout = new QHBoxLayout();
    backLayout->addWidget(m_backButton);
    backLayout->setAlignment(m_backButton, Qt::AlignLeft);

    // Adaugă layout-ul butonului „Back” la layout-ul principal
    m_layout->addLayout(backLayout);

    // Setează distanța dintre widgeturi și marginile layout-ului
    m_layout->setSpacing(0); // Distanța dintre grupuri redusă la jumătate
    m_layout->setContentsMargins(50, 30, 50, 30); // Margini laterale și sus/jos

    // Creează câmpurile pentru login
    m_usernameLabel = new QLabel("Username:", this);
    m_passwordLabel = new QLabel("Password:", this);
    m_usernameField = new QLineEdit(this);
    m_passwordField = new QLineEdit(this);
    m_passwordField->setEchoMode(QLineEdit::Password);

    m_submitButton = new QPushButton("Submit", this);

    // Stilizează câmpurile, butonul și etichetele
    QString fieldStyle = R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 200); /* Alb transparent */
            color: black; /* Culoarea textului */
            border: 2px solid black; /* Graniță neagră */
            border-radius: 10px; /* Colțuri rotunjite */
            padding: 5px; /* Spațiere internă */
            font-size: 16px; /* Dimensiunea fontului */
        }
        QPushButton {
            background-color: rgba(255, 255, 0, 200); /* Galben transparent */
            color: black; /* Culoarea textului */
            font-size: 18px; /* Dimensiunea fontului */
            font-weight: bold; /* Text îngroșat */
            border-radius: 10px; /* Colțuri rotunjite */
            padding: 10px; /* Spațiere internă */
        }
    )";
    m_usernameField->setStyleSheet(fieldStyle);
    m_passwordField->setStyleSheet(fieldStyle);
    m_submitButton->setStyleSheet(fieldStyle);

    QString labelStyle = R"(
        QLabel {
            color: rgba(0, 0, 0, 255); /* Text negru */
            font-size: 18px; /* Dimensiunea fontului */
            font-weight: bold; /* Text îngroșat */
            margin: 0px; /* Fără margini */
            padding: 0px; /* Fără padding */
        }
    )";
    m_usernameLabel->setStyleSheet(labelStyle);
    m_passwordLabel->setStyleSheet(labelStyle);

    // Setează dimensiuni fixe pentru câmpuri și buton
    m_usernameField->setFixedSize(300, 40);
    m_passwordField->setFixedSize(300, 40);
    m_submitButton->setFixedSize(200, 50);

    // Creează layout-uri intermediare pentru fiecare etichetă și câmp
    QVBoxLayout* usernameLayout = new QVBoxLayout();
    usernameLayout->addWidget(m_usernameLabel, 0, Qt::AlignCenter); // Centrează eticheta
    usernameLayout->addWidget(m_usernameField, 0, Qt::AlignCenter); // Centrează câmpul
    usernameLayout->setSpacing(0); // Fără spațiu între etichetă și câmp
    usernameLayout->setContentsMargins(0, 0, 0, 0); // Fără margini

    QVBoxLayout* passwordLayout = new QVBoxLayout();
    passwordLayout->addWidget(m_passwordLabel, 0, Qt::AlignCenter); // Centrează eticheta
    passwordLayout->addWidget(m_passwordField, 0, Qt::AlignCenter); // Centrează câmpul
    passwordLayout->setSpacing(0); // Fără spațiu între etichetă și câmp
    passwordLayout->setContentsMargins(0, 0, 0, 0); // Fără margini

    // Adaugă layout-urile în layout-ul principal și aliniază-le la centru
    m_layout->addLayout(usernameLayout);
    m_layout->setAlignment(usernameLayout, Qt::AlignCenter); // Centrează vertical
    m_layout->addLayout(passwordLayout);
    m_layout->setAlignment(passwordLayout, Qt::AlignCenter); // Centrează vertical
    m_layout->addWidget(m_submitButton);
    m_layout->setAlignment(m_submitButton, Qt::AlignCenter); // Centrează butonul
}



void MainWindow::ShowSignInForm() {
    // Curăță layout-ul
    ClearLayout();

    // Creează butonul „Back”
    m_backButton = new QPushButton("Back", this);
    QString backButtonStyle = R"(
        QPushButton {
            background-color: rgba(200, 200, 200, 200);
            color: black;
            font-size: 12px;
            border: 1px solid black;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: rgba(180, 180, 180, 200);
        }
    )";
    m_backButton->setStyleSheet(backButtonStyle);
    m_backButton->setFixedSize(50, 30);

    // Conectează butonul „Back” la metoda pentru ecranul principal
    connect(m_backButton, &QPushButton::clicked, this, &MainWindow::ShowMainScreen);

    // Creează un layout pentru butonul „Back” și poziționează-l în colțul stânga-sus
    QHBoxLayout* backLayout = new QHBoxLayout();
    backLayout->addWidget(m_backButton);
    backLayout->setAlignment(m_backButton, Qt::AlignLeft);

    // Adaugă layout-ul butonului „Back” la layout-ul principal
    m_layout->addLayout(backLayout);
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

    // Stilizează câmpurile și etichetele
    QString fieldStyle = R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 200); /* Alb transparent */
            color: black; /* Culoarea textului */
            border: 2px solid black; /* Graniță neagră */
            border-radius: 10px; /* Colțuri rotunjite */
            padding: 5px; /* Spațiere internă */
            font-size: 16px; /* Dimensiunea fontului */
        }
        QPushButton {
            background-color: rgba(255, 255, 0, 200); /* Galben transparent */
            color: black; /* Culoarea textului */
            font-size: 18px; /* Dimensiunea fontului */
            font-weight: bold; /* Text îngroșat */
            border-radius: 10px; /* Colțuri rotunjite */
            padding: 10px; /* Spațiere internă */
        }
    )";
    m_usernameField->setStyleSheet(fieldStyle);
    m_passwordField->setStyleSheet(fieldStyle);
    m_confirmPasswordField->setStyleSheet(fieldStyle);
    m_submitButton->setStyleSheet(fieldStyle);

    QString labelStyle = R"(
        QLabel {
            color: rgba(0, 0, 0, 255); /* Negru intens */
            font-size: 18px; /* Dimensiunea fontului */
            font-weight: bold; /* Text îngroșat */
            margin: 0px; /* Fără margini */
            padding: 0px; /* Fără padding */
        }
    )";
    m_usernameLabel->setStyleSheet(labelStyle);
    m_passwordLabel->setStyleSheet(labelStyle);
    m_confirmPasswordLabel->setStyleSheet(labelStyle);

    // Setează dimensiuni fixe pentru câmpuri și buton
    m_usernameField->setFixedSize(300, 40);
    m_passwordField->setFixedSize(300, 40);
    m_confirmPasswordField->setFixedSize(300, 40);
    m_submitButton->setFixedSize(200, 50);

    // Creează layout-uri intermediare pentru fiecare etichetă și câmp
    QVBoxLayout* usernameLayout = new QVBoxLayout();
    usernameLayout->addWidget(m_usernameLabel, 0, Qt::AlignCenter); // Centrează eticheta
    usernameLayout->addWidget(m_usernameField, 0, Qt::AlignCenter); // Centrează câmpul
    usernameLayout->setSpacing(0); // Fără spațiu între etichetă și câmp
    usernameLayout->setContentsMargins(0, 0, 0, 0); // Fără margini

    QVBoxLayout* passwordLayout = new QVBoxLayout();
    passwordLayout->addWidget(m_passwordLabel, 0, Qt::AlignCenter); // Centrează eticheta
    passwordLayout->addWidget(m_passwordField, 0, Qt::AlignCenter); // Centrează câmpul
    passwordLayout->setSpacing(0);
    passwordLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* confirmPasswordLayout = new QVBoxLayout();
    confirmPasswordLayout->addWidget(m_confirmPasswordLabel, 0, Qt::AlignCenter); // Centrează eticheta
    confirmPasswordLayout->addWidget(m_confirmPasswordField, 0, Qt::AlignCenter); // Centrează câmpul
    confirmPasswordLayout->setSpacing(0);
    confirmPasswordLayout->setContentsMargins(0, 0, 0, 0);

    // Adaugă layout-urile în layout-ul principal și aliniază-le la centru
    m_layout->addLayout(usernameLayout);
    m_layout->setAlignment(usernameLayout, Qt::AlignCenter); // Centrează grupul
    m_layout->addLayout(passwordLayout);
    m_layout->setAlignment(passwordLayout, Qt::AlignCenter); // Centrează grupul
    m_layout->addLayout(confirmPasswordLayout);
    m_layout->setAlignment(confirmPasswordLayout, Qt::AlignCenter); // Centrează grupul
    m_layout->addWidget(m_submitButton);
    m_layout->setAlignment(m_submitButton, Qt::AlignCenter); // Centrează butonul
}
void MainWindow::ShowMainScreen() {
    // Curăță toate widget-urile și layout-ul
    ClearLayout();

    // Re-creează titlul
    QLabel* titleLabel = new QLabel("Monkeys Madness", this);
    QFont titleFont("Arial", 36, QFont::Bold); // Font mare și bold
    titleLabel->setFont(titleFont);

    // Setează culoarea textului și stilizare
    titleLabel->setStyleSheet("color: rgba(255, 223, 0, 255);");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Creează efectul de umbră pentru titlu
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    titleLabel->setGraphicsEffect(shadowEffect);

    // Adaugă titlul în layout
    m_layout->addWidget(titleLabel);
    m_layout->setAlignment(titleLabel, Qt::AlignCenter);

    // Re-creează butoanele Login și Sign In
    m_loginButton = new QPushButton("Login", this);
    m_signInButton = new QPushButton("Sign In", this);

    // Stilizare butoane
    QString buttonStyle = R"(
        QPushButton {
            background-color: rgba(255, 255, 0, 200);
            color: black;
            font-size: 18px;
            font-weight: bold;
            border-radius: 5px;
            padding: 5px;
        }
        QPushButton:hover {
            background-color: rgba(255, 215, 0, 230);
        }
    )";
    m_loginButton->setStyleSheet(buttonStyle);
    m_signInButton->setStyleSheet(buttonStyle);

    // Setează dimensiunile butoanelor
    m_loginButton->setFixedSize(200, 50);
    m_signInButton->setFixedSize(200, 50);

    // Adaugă butoanele la layout
    m_layout->addWidget(m_loginButton);
    m_layout->addWidget(m_signInButton);

    // Centrează butoanele
    m_layout->setAlignment(m_loginButton, Qt::AlignCenter);
    m_layout->setAlignment(m_signInButton, Qt::AlignCenter);

    // Conectează butoanele la sloturile lor respective
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::ShowLoginForm);
    connect(m_signInButton, &QPushButton::clicked, this, &MainWindow::ShowSignInForm);
}
