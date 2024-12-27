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

class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget* parent = nullptr);
signals:
    void lobbyWindowClosed(); // Semnal personalizat pentru închiderea ferestrei

private:
    QLabel* m_titleLabel;
    QPushButton* m_playButton;
    QPushButton* m_createLobbyButton;
    QPushButton* m_quitButton;
    QListWidget* m_lobbyList;

    void setupUI();

private slots:
    void onPlayButtonClicked();
    void onCreateLobbyButtonClicked();
    void onQuitButtonClicked();
};

#endif // LOBBYWINDOW_H
