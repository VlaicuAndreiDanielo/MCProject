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
#include "Player.h"
class EndGameWindow : public QWidget {
public:
	EndGameWindow(Player* player, QWidget* parent);

private slots:

	void OnButtonClicked();

    void OnPointUpgradeClicked();

	void UpdateCountdown();

private:
	Player* m_player;
	QProgressBar* m_pointBar;
	QLabel* m_countdownLabel;
	QPushButton* m_button;
	QTimer* m_timer;
};