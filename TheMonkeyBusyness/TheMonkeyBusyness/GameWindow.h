#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "Player.h"
#include "Arena.h"
#include "Raycast.h";
class GameWindow : public QWidget { //Qt graphic interface 
public:
    explicit GameWindow(QWidget* parent = nullptr);
private:
    void paintEvent(QPaintEvent* event) override;
    Player player;
    QTimer* timer;
    void CycleBullets(Player& player);
};