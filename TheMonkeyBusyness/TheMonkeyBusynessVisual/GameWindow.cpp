#include "GameWindow.h"
#include <QtCore/QTimer>
#include <QtCore/qobject.h>
#include <qpainter.h>
GameWindow::GameWindow(QWidget* parent) : QWidget(parent)
{
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(&playerInput);
	timer = new QTimer(this);
    //Bucata comentata de mai jos trebuie mutata in server la initializarea playerului

    /*Character* character = new BasicMonkey();
    player.SetMonkeyType(character);
    player.SetSpawn(Vector2(Arena::Instance().GetSpawn().first * SQUARE_SIZE + SQUARE_SIZE / 2, Arena::Instance().GetSpawn().second * SQUARE_SIZE + SQUARE_SIZE / 2));*/
	
    QObject::connect(timer, &QTimer::timeout, [this]() {

        //Mario aici trebuie sa ajunga inputul de la server pt a muta playerul. Functiile de player.UpdateRotation si partea de raycast trebuie facute pe server
        //Le pot face eu daca imi comentezi unde

        /*if (GameObject* hit = Cast::Raycast(player.GetPosition(), player.ReturnInputHandler()->m_direction, 15); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
            if (tempTile->getType() != TileType::DestructibleWall && tempTile->getType() != TileType::IndestructibleWall) {
                player.UpdatePosition(player.ReturnInputHandler()->m_direction);
            }
        }
        player.UpdateRotation(player.ReturnInputHandler()->m_mousePosition, width(), height());
        player.Shoot(player.ReturnInputHandler()->m_mousePosition, width(), height());
        CycleBullets(player);*/

		update();

        //Mario aici trebuie trimis inputul de playerului inapoi la server ex:
        /*playerInput.m_direction; 
        playerInput.m_mousePosition;
        playerInput.is_shooting;*/

        //Astea isi dau update la fiecare tick al timerului deci 16ms
        //m_direction si m_mousePosition trebuie convertite in 2 floaturi
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

    // Aici voi Updata pozitia playerului primita de la server

   /* int playerX = player.GetPosition().x;
    int playerY = player.GetPosition().y;*/

    // Define the size of the visible area 
    int viewWidth = screenWidth;
    int viewHeight = screenHeight;

    // Translate the painter's coordinate system to shift the map, centering the player

    //Voi muta functiile de draw din player si arena direct in GameWindow

    /*painter.translate(screenWidth / 2 - playerX, screenHeight / 2 - playerY);
    Arena::Instance().draw(painter);
   
    player.m_weapon.DrawBullets(painter);

    player.draw(painter);*/

   
    // Restore painter to its previous state
    painter.restore();
}

//Trebuie mutata implementarea asta in Bullet

//void GameWindow::CycleBullets(Player& player)
//{
//    auto& bullets = player.m_weapon.getBullets();
//    for (auto it = bullets.begin(); it != bullets.end(); /* no increment here */) {
//        Bullet& bullet = *it;
//        if (GameObject* hit = Cast::Raycast(bullet.GetPosition(), bullet.GetDirection(), 5); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
//            if (tempTile->getType() == TileType::DestructibleWall || tempTile->getType() == TileType::IndestructibleWall) {
//                it = bullets.erase(it);
//            }
//            else {
//                it++;
//            }
//        }
//        else {
//            it++;
//        }
//    }
//}
