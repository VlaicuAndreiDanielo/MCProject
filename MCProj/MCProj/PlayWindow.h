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

class PlayWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PlayWindow(QWidget* parent = nullptr);

private:
    QPushButton* m_playButton; // Butonul Play
    QPushButton* m_quitButton;   // Butonul Quit Game
};

#endif // PLAYWINDOW_H
