#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void showLoginForm();
    void showSignInForm();

private:
    QWidget* centralWidget;
    QVBoxLayout* layout;

    // Prima interfață
    QPushButton* loginButton;
    QPushButton* signInButton;

    // Elemente pentru formular
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* confirmPasswordLabel;
    QLineEdit* usernameField;
    QLineEdit* passwordField;
    QLineEdit* confirmPasswordField;
    QPushButton* submitButton;

    // Funcție de resetare
    void clearLayout();
};

#endif // MAINWINDOW_H
