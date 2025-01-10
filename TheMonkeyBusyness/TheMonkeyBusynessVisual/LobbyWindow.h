/*Versiunea cu QListWidget pt lobbyuri*/
#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qscreen.h>
#include <qmessagebox.h>
#include <QCoreApplication>
#include <QPixmap>
#include <QPalette>
#include <QSizePolicy>
#include <QInputDialog> 
#include <qdatetime.h>
#include "GameWindow.h"


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

