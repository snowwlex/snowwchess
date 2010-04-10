#include <QMessageBox>
#include <QDebug>
#include <QtXml>

#include "snowwchess.h"
#include "rules.h"
#include "rules_io.h"
#include "model.h"
#include "player.h"

#include "guiboardview.h"
#include "game.h"


#include "mainwindow.h"
#include "dialognewgame.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionNewGame, SIGNAL(activated()),SLOT(slotActionNewGameActivated()));



}

mainWindow::~mainWindow() {

}

void mainWindow::slotActionNewGameActivated() {

	RulesIO rulesIO;

	dialogNewGame* dialog = new dialogNewGame();
	if (dialog->exec() == QDialog::Accepted) {
		bool ok = rulesIO.load(dialog->getRulesFileName().toStdString());
		if (ok == false) {
			 QMessageBox(QMessageBox::Warning, tr("Error"), tr("File with rules is not valid"), QMessageBox::Ok).exec();
		}
	}
	delete dialog;

	if (dialog->exec() != QDialog::Accepted) {
		return;
	}

	myGame.setRules(rulesIO);

	myGame.setPlayer(WHITE, new HumanPlayer() );
	myGame.setPlayer(BLACK, new HumanPlayer() );

	if (myGame.isReady() == false) {
		myGame.clear();
		qDebug() << "Game is not ready";
		return;
	}

	myGame.start();

}
