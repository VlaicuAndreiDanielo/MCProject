﻿#ifndef SIGNINFORM_H
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
    void backRequested(); // Semnal pentru revenirea la fereastra anterioară

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
