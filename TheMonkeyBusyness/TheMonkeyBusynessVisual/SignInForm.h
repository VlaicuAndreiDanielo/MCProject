#ifndef SIGNINFORM_H
#define SIGNINFORM_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPalette>
#include <QCoreApplication>
#include <qmessagebox.h>
#include <QVBoxLayout>

class SignInForm : public QDialog {
    Q_OBJECT

public:
    explicit SignInForm(QWidget* parent = nullptr);

signals:
    void BackRequested(); // Semnal pentru revenirea la fereastra anterioară
    void SessionStarted(); //Semnal pt a inchide si fereastra principala cu butoane
private:
    QLabel* m_usernameLabel;
    QLabel* m_passwordLabel;
    QLabel* m_confirmPasswordLabel;
    QLineEdit* m_usernameField;
    QLineEdit* m_passwordField;
    QLineEdit* m_confirmPasswordField;
    QPushButton* m_submitButton;
    QPushButton* m_backButton;
};

#endif // SIGNINFORM_H
