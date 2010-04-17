#ifndef SNOWWCHESS_H
#define SNOWWCHESS_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:
		void on_actionNewGame_activated();

	private slots:

	signals:

	private:
		Ui::mainWindowClass ui;

	private:

		Game myGame;


};

#endif // SNOWWCHESS_H
