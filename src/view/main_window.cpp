#include <QMessageBox>
#include <QtXml>
#include <QLayout>
#include <QFileDialog>

#include <cassert>

#include "main_window.h"
#include "dialog_new_game.h"
#include "../model/io/rules_io.h"
#include "../model/io/model_io.h"
#include "../player/human_player.h"
#include "../player/alphabeta_parallel_search_ai_player.h"
#include "../player/alphabeta_search_ai_player.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	myGame = 0;
	myModel = 0;
	myRules = 0;
	myPlayers[WHITE] = 	myPlayers[BLACK] = 0;

	connect(ui.pushButton, SIGNAL( clicked() ), ui.historyView, SLOT( makeUndo() ) );

}

MainWindow::~MainWindow() {
	delete myModel;
	delete myRules;
	delete myGame;
}

void MainWindow::on_actionSaveGame_activated() {
	if (myModel == 0) {
		qDebug() << ":MainWindow: " << "model is not setted";
		return;
	}

	ModelIO modelIO;
	QString file = QFileDialog::getSaveFileName(this, "Save board to", "saves", tr("Saves in xml (*.xml)"));
	modelIO.save( file.toStdString(), *myModel );
	QMessageBox(QMessageBox::Information, tr("Ok!"), tr("File with board is saved"), QMessageBox::Ok).exec();

}


void MainWindow::loadGame(DialogNewGame* dialog) {
	RulesIO rulesIO;
	ModelIO modelIO;

	modelIO.load( dialog->getFileName().toStdString() );
	bool ok = rulesIO.load( "rules/"+modelIO.getStorage().rulesName+".xml" );
	if (ok == false) {
		 QMessageBox(QMessageBox::Warning, tr("Error"), tr("Can't load, because file with rules is not valid"), QMessageBox::Ok).exec();
		 return;
	}

	delete myRules;
	delete myModel;
	myRules = new Rules;
	myModel = new Model;

	rulesIO.updateRules(*myRules);
	myModel->setRules(myRules);
	modelIO.updateModel(*myModel);
	myModel->init(false);

	for (int i = 0; i < 2; ++i) {
		switch ( dialog->getPlayerType(i) ) {
			case HUMAN:
				myPlayers[i] = new HumanPlayer();
				break;
			case AI:
				AIPlayer* player;
				switch ( dialog->getAILevel(i) )  {
					case 0:
						player =  new AlphaBetaParallelSearchAIPlayer(2,0);
						break;
					case 1:
						player =  new AlphaBetaParallelSearchAIPlayer(3,10);
						break;
					case 2:
						player =  new AlphaBetaParallelSearchAIPlayer(4,10);
						break;
				}
				player->setABEqualPruning(true);
				myPlayers[i] = player;
				break;
		}
	}


	for (int i = 0; i < 2; ++i) {
		myPlayers[i]->setModel(myModel);
		myPlayers[i]->setColor(i);
		myPlayers[i]->setBoardView(ui.boardView);
	}

	delete myGame;
	myGame = new Game;
	myGame->setModel(myModel);
	myGame->setPlayer(WHITE, myPlayers[WHITE] );
	myGame->setPlayer(BLACK, myPlayers[BLACK] );
	myGame->setBoardView(ui.boardView);
	myGame->setHistoryView(ui.historyView);
}


void MainWindow::newGame(DialogNewGame* dialog) {

	RulesIO rulesIO;
	ModelIO modelIO;

	bool ok = rulesIO.load(dialog->getFileName().toStdString());
	if (ok == false) {
		 QMessageBox(QMessageBox::Warning, tr("Error"), tr("File with rules is not valid"), QMessageBox::Ok).exec();
		 return;
	}

	delete myRules;
	delete myModel;
	myRules = new Rules;
	myModel = new Model;

	rulesIO.updateRules(*myRules);
	myModel->setRules(myRules);
	myModel->init(true);

	for (int i = 0; i < 2; ++i) {
		switch ( dialog->getPlayerType(i) ) {
			case HUMAN:
				myPlayers[i] = new HumanPlayer();
				break;
			case AI:
				AIPlayer* player;
				switch ( dialog->getAILevel(i) )  {
					case 0:
						player =  new AlphaBetaParallelSearchAIPlayer(2,0);
						break;
					case 1:
						player =  new AlphaBetaParallelSearchAIPlayer(3,10);
						break;
					case 2:
						player =  new AlphaBetaParallelSearchAIPlayer(4,10);
						break;
				}
				player->setABEqualPruning(true);
				myPlayers[i] = player;
				break;
		}
	}


	for (int i = 0; i < 2; ++i) {
		myPlayers[i]->setModel(myModel);
		myPlayers[i]->setColor(i);
		myPlayers[i]->setBoardView(ui.boardView);
	}

	delete myGame;
	myGame = new Game;
	myGame->setModel(myModel);
	myGame->setPlayer(WHITE, myPlayers[WHITE] );
	myGame->setPlayer(BLACK, myPlayers[BLACK] );
	myGame->setBoardView(ui.boardView);
	myGame->setHistoryView(ui.historyView);

}

void MainWindow::on_actionNewGame_activated() {

	DialogNewGame* dialog = new DialogNewGame();
	dialog->setLoadFileText( tr("Opening file with rules"), "rules", tr("Rules in xml (*.xml)") );
	if (dialog->exec() != QDialog::Accepted) {
		delete dialog;
		return;
	}

	newGame(dialog);
	delete dialog;
	myGame->init();

}

void MainWindow::on_actionLoadGame_activated() {

	DialogNewGame* dialog = new DialogNewGame();
	dialog->setLoadFileText( tr("Opening file with board"), "saves", tr("Board in xml (*.xml)") );
	if (dialog->exec() != QDialog::Accepted) {
		delete dialog;
		return;
	}
	loadGame(dialog);

	delete dialog;
	myGame->init();

}
