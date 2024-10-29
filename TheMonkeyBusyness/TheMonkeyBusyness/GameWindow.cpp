#include "GameWindow.h"

GameWindow::GameWindow(QWidget* parent) : QWidget(parent)
{
	setFixedSize(400, 300);
	setFocusPolicy(Qt::StrongFocus);
}
