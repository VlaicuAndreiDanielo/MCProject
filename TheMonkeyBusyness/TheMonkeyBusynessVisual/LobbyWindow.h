/*Versiunea cu QListWidget pt lobbyuri*/
#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QPixmap>
#include <QtGui/QPalette>        
#include <QtWidgets/QSizePolicy> 
#include <QtWidgets/QInputDialog>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>


class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(int playerId, QWidget* parent = nullptr);
signals:
    void LobbyWindowClosed(); // Semnal personalizat pentru închiderea ferestrei

private:
    QLabel* m_titleLabel;
    QPushButton* m_playButton;
    QPushButton* m_createLobbyButton;
    QPushButton* m_quitButton;
    QListWidget* m_lobbyList;
    int m_playerId;
    void SetupUI();

private slots:
    void OnPlayButtonClicked();
    void OnCreateLobbyButtonClicked();
    void OnQuitButtonClicked();
};

#endif // LOBBYWINDOW_H

