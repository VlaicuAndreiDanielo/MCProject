#pragma once
#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "Player.h"
class GameWindow : public QWidget { //Qt graphic interface 
public:
    explicit GameWindow(QWidget* parent = nullptr);
private:

    void paintEvent(QPaintEvent* event) override;

    Player player;
    QTimer* timer;

};