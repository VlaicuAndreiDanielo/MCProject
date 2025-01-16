#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include <cmath>
#include <vector>
#include <unordered_set>
class InputHandler : public QWidget { //this class reads key input
public:
    explicit InputHandler(QWidget* parent = nullptr);
    QPoint m_direction;
    QPoint m_mousePosition;
    bool is_shooting;
    bool is_specialAbility;
    void Normalize(QPoint& point);
private:
    std::unordered_map<int, bool> keyStates;
protected:
    // Override eventFilter to process key events
    bool eventFilter(QObject* obj, QEvent* event) override;
};
