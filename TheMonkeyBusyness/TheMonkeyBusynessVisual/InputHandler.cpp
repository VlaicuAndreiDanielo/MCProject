#include "InputHandler.h"

InputHandler::InputHandler(QWidget* parent) : QWidget(parent), is_shooting(false) {

}

void InputHandler::Normalize(QPoint& point)
{
    float magnitude = (float)std::sqrt(pow(point.x(), 2) + pow(point.y(), 2));
    if (!magnitude == 0)  (point /= magnitude);
}

// Override the eventFilter to handle key events
bool InputHandler::eventFilter(QObject* obj, QEvent* event) {
    // Check if the event is a key press
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();

        if (event->type() == QEvent::KeyPress) {
            keyStates[key] = true; // Mark the key as pressed
        }
        else if (event->type() == QEvent::KeyRelease) {
            keyStates[key] = false; // Mark the key as released
        }

        m_direction.setX(0);
        m_direction.setY(0);

        if (keyStates[Qt::Key_W]) {
            m_direction.setY(m_direction.y() - 1);
        }
        if (keyStates[Qt::Key_A]) {
            m_direction.setX(m_direction.x() - 1);
        }
        if (keyStates[Qt::Key_S]) {
            m_direction.setY(m_direction.y() + 1);
        }
        if (keyStates[Qt::Key_D]) {
            m_direction.setX(m_direction.x() + 1);
        }


        Normalize(m_direction);
        return true;
        // Indicate that the event was handled
    }
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint mousePoint = mouseEvent->pos();
        m_mousePosition = mousePoint;
        /*std::cout << mousePosition;
        std::cout << std::endl;*/
    }
    if (event->type() == QEvent::MouseButtonPress) {
        is_shooting = true;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        is_shooting = false;
    }
    // Pass the event on to the base class
    return QObject::eventFilter(obj, event);
}

