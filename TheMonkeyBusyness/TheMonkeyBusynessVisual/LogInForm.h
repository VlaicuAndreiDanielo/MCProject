#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QtWidgets/QDialog>                
#include <QtWidgets/QPushButton>           
#include <QtWidgets/QLineEdit>            
#include <QtWidgets/QLabel>               
#include <QtWidgets/QVBoxLayout>          
#include <QtWidgets/QGraphicsDropShadowEffect> 
#include <QtGui/QPixmap>                   
#include <QtGui/QPalette>                  
#include <QtWidgets/QMessageBox>          
#include <QtCore/QCoreApplication>


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
