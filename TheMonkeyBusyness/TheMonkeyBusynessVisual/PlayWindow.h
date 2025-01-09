#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QCoreApplication>
#include "LobbyWindow.h"

class PlayWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PlayWindow(QWidget* parent = nullptr);
private slots:
    void OpenLobbyWindow(); // Slot pentru deschiderea ferestrei Lobby
    void HandleLobbyWindowClosed(); // Gestionarea închiderii LobbyWindow
    void ShowControls(); //pt afisarea controalelor
private:
    QLabel* m_titleLabel;
    QVBoxLayout* m_layout;
    QPushButton* m_playButton; // Butonul Play
    QPushButton* m_quitButton;   // Butonul Quit Game
    LobbyWindow* m_lobbyWindow; // Pointer pentru LobbyWindow
    QPushButton* m_controlsButton;  // Buton pt controlae
};

#endif // PLAYWINDOW_H
