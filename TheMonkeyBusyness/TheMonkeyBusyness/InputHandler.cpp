#include "InputHandler.h"

InputHandler::InputHandler(QWidget* parent) : QWidget(parent) {}

// Override the eventFilter to handle key events
bool InputHandler::eventFilter(QObject* obj, QEvent* event) {
    // Check if the event is a key press
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // Check which key was pressed
        switch (keyEvent->key()) {
        case Qt::Key_A:
            qDebug() << "Key A was pressed";
            break;
        case Qt::Key_S:
            qDebug() << "Key S was pressed";
            break;
        case Qt::Key_D:
            qDebug() << "Key D was pressed";
            break;
        case Qt::Key_W:
            qDebug() << "Key W was pressed";
            break;
        default:
            break;
        }
        
        return true; // Indicate that the event was handled
    }

    // Pass the event on to the base class
    return QObject::eventFilter(obj, event);
}
