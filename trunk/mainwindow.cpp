#include <QMessageBox>
#include <QDebug>
#include <QtXml>
#include <QLayout>

#include "snowwchess.h"
#include "rules.h"
#include "rules_io.h"
#include "model.h"
#include "player.h"

#include "guiboardview.h"
#include "game.h"


#include "mainwindow.h"
#include "dialognewgame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	//ui.centralwidget->set
}

MainWindow::~MainWindow() {

}

void MainWindow::on_actionNewGame_activated() {

	RulesIO rulesIO;

	DialogNewGame* dialog = new DialogNewGame();
	bool accepted = false;
	if (dialog->exec() == QDialog::Accepted) {
		accepted = true;
		qDebug() << "accepted";
		bool ok = rulesIO.load(dialog->getRulesFileName().toStdString());
		if (ok == false) {
			 QMessageBox(QMessageBox::Warning, tr("Error"), tr("File with rules is not valid"), QMessageBox::Ok).exec();
		}
		else {
			qDebug() << "rules file is valid";
		}
	}
	delete dialog;

	if (accepted != true) {
		qDebug() << "not accepted";
		return;
	}

	myGame.loadRules(rulesIO);
	myGame.setPlayer(WHITE, new HumanPlayer() );
	myGame.setPlayer(BLACK, new HumanPlayer() );
	myGame.setBoardView(ui.boardView);

	myGame.prepare();
	if (myGame.isReady() == false) {
		qDebug() << "Game is not ready";
		myGame.clear();
		return;
	}

	myGame.start();

}
