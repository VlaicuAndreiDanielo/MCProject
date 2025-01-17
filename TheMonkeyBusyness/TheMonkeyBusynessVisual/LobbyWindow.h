/*Versiunea cu QListWidget pt lobbyuri*/
#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include "Player.h"
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
#include <QtCore/qtimer.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <map>

class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(int playerId, QWidget* parent = nullptr);
    ~LobbyWindow();
signals:
    void LobbyWindowClosed(); // Semnal personalizat pentru închiderea ferestrei

   
private:
    QTimer* m_timer;
    ix::WebSocket* webSocket;
    bool startedGame = false;
    bool host = false;
    QLabel* m_titleLabel;
    QPushButton* m_playButton;
    QPushButton* m_createLobbyButton;
    QPushButton* m_quitButton;
    QListWidget* m_lobbyList;
    std::map<QListWidgetItem*, int> m_lobbyData;
    //std::unordered_map<>
    //std::string serverUrl = "http://localhost:8080";
    Player* m_player;
    int m_playerId;
    void CheckStart();
    void SetupUI();
    void GetLobbiesFromServer();
    void GetLobbyData(std::vector<int> lobbyIds);
    void GetLobbyData(int id);
    void LoadLobby(int id, int playerCount);
private slots:
    void OnPlayButtonClicked();
    void OnCreateLobbyButtonClicked();
    void OnQuitButtonClicked();
    void OnItemClicked(QListWidgetItem* item);
};

#endif // LOBBYWINDOW_H

