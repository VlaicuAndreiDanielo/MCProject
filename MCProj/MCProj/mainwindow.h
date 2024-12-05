#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect> // Include necesar pentru efectul de umbră


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void ShowLoginForm();
    void ShowSignInForm();
    void ShowMainScreen();

private:
    QWidget* m_centralWidget;
    QVBoxLayout* m_layout;

    // Prima interfață
    QPushButton* m_loginButton;
    QPushButton* m_signInButton;
    QPushButton* m_backButton; // Buton pentru revenire la ecranul principal

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
