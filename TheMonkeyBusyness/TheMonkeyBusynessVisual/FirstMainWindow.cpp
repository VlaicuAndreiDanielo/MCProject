
#include "LoginForm.h"
#include "SignInForm.h"
#include "FirstMainWindow.h"

FirstMainWindow::FirstMainWindow(QWidget* parent) : QMainWindow(parent), m_loginForm(nullptr), m_signInForm(nullptr) {
    setFixedSize(700, 700); // Setează dimensiunea fixă pentru fereastră
    // Creează widgetul central și layout-ul
    m_centralWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centralWidget);
    m_centralWidget->setLayout(m_layout);
    setCentralWidget(m_centralWidget);

    // **Setează imaginea ca fundal**
    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background.jpg";
    QPixmap background(imagePath);
    if (!background.isNull()) {
        background = background.scaled(size(), Qt::KeepAspectRatioByExpanding);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(background));
        setPalette(palette);
    }
    else {
        qDebug() << "Imaginea nu a fost încărcată corect!";
    }

    // Încarcă ecranul principal
    ShowMainScreen();
}

void FirstMainWindow::ShowMainScreen() {
    ClearLayout();

    // Creează titlul
    QLabel* titleLabel = new QLabel("Monkeys Madness", this);
    QFont titleFont("Arial", 36, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: rgba(255, 223, 0, 255);");
    titleLabel->setAlignment(Qt::AlignCenter);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    titleLabel->setGraphicsEffect(shadowEffect);

    m_layout->addWidget(titleLabel);
    m_layout->setAlignment(titleLabel, Qt::AlignCenter);

    // Creează butoanele Login și Sign In
    m_loginButton = new QPushButton("Login", this);
    m_signInButton = new QPushButton("Sign In", this);

    QString buttonStyle = R"(
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
    m_loginButton->setStyleSheet(buttonStyle);
    m_signInButton->setStyleSheet(buttonStyle);

    m_loginButton->setFixedSize(200, 50);
    m_signInButton->setFixedSize(200, 50);

    m_layout->addWidget(m_loginButton);
    m_layout->addWidget(m_signInButton);

    m_layout->setAlignment(m_loginButton, Qt::AlignCenter);
    m_layout->setAlignment(m_signInButton, Qt::AlignCenter);

    // Conectează butoanele la sloturile lor respective
    connect(m_loginButton, &QPushButton::clicked, this, &FirstMainWindow::ShowLoginForm);
    connect(m_signInButton, &QPushButton::clicked, this, &FirstMainWindow::ShowSignInForm);
}

void FirstMainWindow::ShowLoginForm() {
    if (!m_loginForm) { // Creează fereastra doar dacă nu există deja
        m_loginForm = new LoginForm(this);
        m_loginForm->setModal(false);
        m_loginForm->setAttribute(Qt::WA_DeleteOnClose); // Șterge automat fereastra la închidere

        // Conectare pentru butonul Back
        connect(m_loginForm, &LoginForm::BackRequested, [=]() {
            m_loginForm->close(); // Închide fereastra Login
            m_loginForm = nullptr; // Resetează pointerul
            ShowMainScreen(); // Reveniți la ecranul principal
            });

        // Conectare pentru sesiune începută
        connect(m_loginForm, &LoginForm::SessionStarted, [=]() {
            m_loginForm->close(); // Închide LoginForm
            m_loginForm = nullptr; // Resetează pointerul
            close(); // Închide MainWindow
            });
    }

    // Setează poziția ferestrei copil în funcție de părintele său
    m_loginForm->move(geometry().x(), geometry().y());
    m_loginForm->show();
}

void FirstMainWindow::ShowSignInForm() {
    if (!m_signInForm) { // Creează fereastra doar dacă nu există deja
        m_signInForm = new SignInForm(this);
        m_signInForm->setModal(false);
        m_signInForm->setAttribute(Qt::WA_DeleteOnClose); // Șterge automat fereastra la închidere

        // Conectare pentru butonul Back
        connect(m_signInForm, &SignInForm::BackRequested, [=]() {
            m_signInForm->close(); // Închide fereastra Sign In
            m_signInForm = nullptr; // Resetează pointerul
            ShowMainScreen(); // Reveniți la ecranul principal
            });

        // Conectare pentru sesiune începută
        connect(m_signInForm, &SignInForm::SessionStarted, [=]() {
            m_signInForm->close(); // Închide SignInForm
            m_signInForm = nullptr; // Resetează pointerul
            close(); // Închide MainWindow
            });
    }

    // Setează poziția ferestrei copil în funcție de părintele său
    m_signInForm->move(geometry().x(), geometry().y());
    m_signInForm->show();
}

void FirstMainWindow::ClearLayout() {
    while (m_layout->count() > 0) {
        QLayoutItem* item = m_layout->takeAt(0); // Ia primul element din layout
        if (item->widget()) {                   // Dacă elementul este un widget
            delete item->widget();              // Șterge widgetul
        }
        delete item;                            // Șterge obiectul layout item
    }
}

void FirstMainWindow::moveEvent(QMoveEvent* event) {
    QMainWindow::moveEvent(event);

    // Actualizează poziția ferestrelor copil, dacă există
    if (m_loginForm && m_loginForm->isVisible()) {
        m_loginForm->move(geometry().x(), geometry().y());
    }
    if (m_signInForm && m_signInForm->isVisible()) {
        m_signInForm->move(geometry().x(), geometry().y());
    }
}
