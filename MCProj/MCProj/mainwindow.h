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
    QWidget* mainWidget;
    QVBoxLayout* layout;
    QPushButton* loginButton;
    QPushButton* signInButton;

    // Form elements
    QLineEdit* usernameField;
    QLineEdit* passwordField;
    QLineEdit* confirmPasswordField;
};

#endif // MAINWINDOW_H
