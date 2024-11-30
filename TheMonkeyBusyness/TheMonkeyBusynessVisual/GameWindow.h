#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "InputHandler.h"
class GameWindow : public QWidget { //Qt graphic interface 
public:
    explicit GameWindow(QWidget* parent = nullptr);
private:

    void paintEvent(QPaintEvent* event) override;
    InputHandler playerInput;
    QTimer* timer;
    //temporary color squares for the tiles;
    std::vector<QColor> squareColors{ Qt::green, Qt::black, Qt::red, Qt::yellow, Qt::blue, Qt::darkGreen, Qt::darkRed, Qt::magenta, Qt::yellow };
};