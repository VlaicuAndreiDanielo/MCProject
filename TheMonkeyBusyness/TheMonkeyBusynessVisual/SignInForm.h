#ifndef SIGNINFORM_H
#define SIGNINFORM_H

#include <QtWidgets/QDialog>                
#include <QtWidgets/QPushButton>          
#include <QtWidgets/QLineEdit>             
#include <QtWidgets/QLabel>                
#include <QtWidgets/QVBoxLayout>           
#include <QtWidgets/QGraphicsDropShadowEffect> 
#include <QtGui/QPixmap>                   
#include <QtGui/QPalette>                 
#include <QtCore/QCoreApplication>       
#include <QtWidgets/QMessageBox>

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
