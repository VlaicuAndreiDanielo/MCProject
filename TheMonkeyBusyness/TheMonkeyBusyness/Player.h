#pragma once
#include "InputHandler.h"
#include <QtCore/qobject.h>
class Player : public QObject{ // this is the player, he calls for input and other actions
public:
	explicit Player(QObject* parent = nullptr);
	InputHandler* ReturnInputHandler();
public slots:
	void UpdatePosition(const Vector2 &vector);
	void UpdatePosition(const float x, const float y);
private:
	 InputHandler inputHandler;
	 Vector2 position;
	 
};