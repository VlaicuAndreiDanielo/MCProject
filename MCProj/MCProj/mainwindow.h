#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QCoreApplication>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void ShowLoginForm();
    void ShowSignInForm();

private:
    QWidget* m_centralWidget;
    QVBoxLayout* m_layout;

    // Prima interfață
    QPushButton* m_loginButton;
    QPushButton* m_signInButton;

    // Elemente pentru formular
    QLabel* m_usernameLabel;
    QLabel* m_passwordLabel;
    QLabel* m_confirmPasswordLabel;
    QLineEdit* m_usernameField;
    QLineEdit* m_passwordField;
    QLineEdit* m_confirmPasswordField;
    QPushButton* m_submitButton;

    // Funcție de resetare
    void ClearLayout();
};

#endif // MAINWINDOW_H
