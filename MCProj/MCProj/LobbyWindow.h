#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget* parent = nullptr);

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
