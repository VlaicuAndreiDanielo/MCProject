#include "GameWindow.h"
#include <QtCore/QTimer>
#include <QtCore/qobject.h>
GameWindow::GameWindow(QWidget* parent) : QWidget(parent)
{
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(player.ReturnInputHandler());
	timer = new QTimer(this);
    player.SetSpawn(Vector2(Arena::Instance().GetSpawn().first * SQUARE_SIZE + SQUARE_SIZE / 2, Arena::Instance().GetSpawn().second * SQUARE_SIZE + SQUARE_SIZE / 2));
	QObject::connect(timer, &QTimer::timeout, [this]() {
        if (HitResult hit = Cast::Raycast(player.GetPosition(), player.ReturnInputHandler()->direction, 15); std::holds_alternative<Tile>(hit)) {
            Tile tempTile = std::get<Tile>(hit);
            if (tempTile.getType() != TileType::DestructibleWall && tempTile.getType() != TileType::IndestructibleWall) {
                player.UpdatePosition(player.ReturnInputHandler()->direction);
            }
            player.UpdateRotation(player.ReturnInputHandler()->mousePosition, width(), height());
        }
        player.Shoot(player.ReturnInputHandler()->mousePosition, width(), height());
        CycleBullets(player);
		update();
		});

	timer->start(16);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Save the painter's current state
    painter.save();
    int screenWidth = width();
    int screenHeight = height();

    // Player's position 
    int playerX = player.GetPosition().x;
    int playerY = player.GetPosition().y;

    // Define the size of the visible area 
    int viewWidth = screenWidth;
    int viewHeight = screenHeight;

    // Translate the painter's coordinate system to shift the map, centering the player
    painter.translate(screenWidth / 2 - playerX, screenHeight / 2 - playerY);
    Arena::Instance().draw(painter);
   
    player.weapon.DrawBullets(painter);

    player.draw(painter);

   
    // Restore painter to its previous state
    painter.restore();
}

void GameWindow::CycleBullets(Player& player)
{
    auto& bullets = player.weapon.getBullets();
    for (auto it = bullets.begin(); it != bullets.end(); /* no increment here */) {
        Bullet& bullet = *it;
        if (HitResult hit = Cast::Raycast(bullet.GetPosition(), bullet.GetDirection(), 5); std::holds_alternative<Tile>(hit)) {
            Tile tempTile = std::get<Tile>(hit);
            if (tempTile.getType() == TileType::DestructibleWall || tempTile.getType() == TileType::IndestructibleWall) {
                it = bullets.erase(it);
            }
            else {
                it++;
            }
        }
        else {
            it++;
        }
    }
}
