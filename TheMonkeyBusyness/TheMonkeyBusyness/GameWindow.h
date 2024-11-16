#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "Player.h"
#include "Arena.h"
class GameWindow : public QWidget { //Qt graphic interface 
public:
    explicit GameWindow(QWidget* parent = nullptr);
private:
    std::vector<QColor> squareColors{ Qt::green, Qt::black, Qt::red, Qt::yellow, Qt::blue, Qt::darkGreen, Qt::darkRed, Qt::magenta };
    void paintEvent(QPaintEvent* event) override;
    Arena arena;
    Player player;
    QTimer* timer;

};