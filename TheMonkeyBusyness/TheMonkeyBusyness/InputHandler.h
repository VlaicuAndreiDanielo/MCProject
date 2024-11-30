#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "Vector2.h"
#include <vector>
#include <unordered_set>
class InputHandler : public QWidget { //this class reads key input
public:
    explicit InputHandler(QWidget* parent = nullptr);
    Vector2 m_direction;
    Vector2 m_mousePosition;
    bool is_shooting;
private:
    std::unordered_map<int, bool> keyStates;
protected:
    // Override eventFilter to process key events
    bool eventFilter(QObject* obj, QEvent* event) override;
};