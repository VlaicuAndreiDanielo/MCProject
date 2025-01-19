#pragma once

#include <QtWidgets/QDialog>                
#include <QtWidgets/QPushButton>           
#include <QtWidgets/QLineEdit>            
#include <QtWidgets/QLabel>               
#include <QtWidgets/QVBoxLayout>          
#include <QtWidgets/QGraphicsDropShadowEffect> 
#include <QtGui/QPixmap>                   
#include <QtGui/QPalette>       
#include <qtimer.h>
#include "QtWidgets/qprogressbar.h"      
#include <QtCore/QCoreApplication>
#include <qobject.h>
#include "Player.h"
class EndGameWindow : public QWidget {
	Q_OBJECT
public:
	EndGameWindow(Player* player, QWidget* parent = nullptr);

public slots:

	void OnButtonClicked();

	void UpdateCountdown();

	void EndGameWindowClosed();

private:
	Player* m_player;
	QLabel* m_countdownLabel;
	QPushButton* m_button;
	QTimer* m_timer;
};