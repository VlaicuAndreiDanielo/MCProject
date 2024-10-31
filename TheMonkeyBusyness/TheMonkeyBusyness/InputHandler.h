#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
class InputHandler : public QWidget { //this class reads key input
public:
    explicit InputHandler(QWidget* parent = nullptr);

private:


protected:
    // Override eventFilter to process key events
    bool eventFilter(QObject* obj, QEvent* event) override;
};