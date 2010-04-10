#ifndef SNOWWCHESS_H
#define SNOWWCHESS_H

#include <QtGui/QMainWindow>
#include "guiboardview.h"
#include "ui_mainwindow.h"

#include "snowwchess.h"
#include "rules.h"
#include "rules_io.h"
#include "model.h"
#include "player.h"

#include "game.h"

class mainWindow : public QMainWindow {
    Q_OBJECT

	public:
		mainWindow(QWidget *parent = 0);
		~mainWindow();

	public slots:
		void slotActionNewGameActivated();

	private slots:

	signals:

	private:
		Ui::mainWindowClass ui;

	private:

		Game myGame;


};

#endif // SNOWWCHESS_H
