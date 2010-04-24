#include <QMessageBox>
#include <QtXml>
#include <QLayout>

#include "main_window.h"
#include "dialog_new_game.h"
#include "../model/io/rules_io.h"

#include "../player/human_player.h"
#include "../player/alphabeta_parallel_search_ai_player.h"
#include "../player/alphabeta_search_ai_player.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
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
	myGame.setPlayer(BLACK, new AlphaBetaParallelSearchAIPlayer(4,2) );
	myGame.setBoardView(ui.boardView);

	myGame.prepare();
	if (myGame.isReady() == false) {
		qDebug() << "Game is not ready";
		myGame.clear();
		return;
	}

	myGame.start();

}
