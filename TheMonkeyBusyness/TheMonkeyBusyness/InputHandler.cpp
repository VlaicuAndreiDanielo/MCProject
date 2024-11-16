#include "InputHandler.h"
#include "Vector2.h"
InputHandler::InputHandler(QWidget* parent) : QWidget(parent), is_shooting(false) {

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

        direction.x = 0;
        direction.y = 0;
        
        if (keyStates[Qt::Key_W]) {
            direction.y -= 1;
        }
        if (keyStates[Qt::Key_A]) {
            direction.x -= 1;
        }
        if (keyStates[Qt::Key_S]) {
            direction.y += 1;
        }
        if (keyStates[Qt::Key_D]) {
            direction.x += 1;
        }


        direction.Normalize();
        return true;
       // Indicate that the event was handled
    }
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint mousePoint = mouseEvent->pos();
        mousePosition.x = mousePoint.x();
        mousePosition.y = mousePoint.y();
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
