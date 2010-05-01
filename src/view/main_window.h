#ifndef SNOWWCHESS_H
#define SNOWWCHESS_H

#include <QtGui/QMainWindow>
#include "../../ui_main_window.h"

#include "dialog_new_game.h"
#include "../game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:
		void on_actionNewGame_activated();
		void on_actionSaveGame_activated();
		void on_actionLoadGame_activated();

	private slots:

	signals:

	private:
		void loadGame(DialogNewGame* dialog);
		void newGame(DialogNewGame* dialog);
	private:
		Ui::mainWindowClass ui;

	private:
		Rules* myRules;
		Model* myModel;
		Player* myPlayers[2];
		Game* myGame;


};

#endif // SNOWWCHESS_H
