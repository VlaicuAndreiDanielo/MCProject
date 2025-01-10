#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPalette>
#include <qmessagebox.h>
#include <QCoreApplication>


class LoginForm : public QDialog {
    Q_OBJECT

public:
    explicit LoginForm(QWidget* parent = nullptr);

signals:
    void BackRequested(); // Semnal pentru revenirea la fereastra anterioară
    void SessionStarted(); //Semnal pt a inchide si fereastra principala cu butoane

private:
    QLabel* m_usernameLabel;
    QLabel* m_passwordLabel;
    QLineEdit* m_usernameField;
    QLineEdit* m_passwordField;
    QPushButton* m_submitButton;
    QPushButton* m_backButton;
};

#endif // LOGINFORM_H
