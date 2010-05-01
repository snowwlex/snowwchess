#include <QMessageBox>
#include <cassert>

#include "game.h"


Game::Game() {
	myPlayers[WHITE] = myPlayers[BLACK] = 0;
	myBoardView = 0;
	myHistoryView = 0;
	myModel = 0;
}

Game::~Game() {
	stop();
	delete myPlayers[WHITE];
	delete myPlayers[BLACK];
}

void Game::stop() {
	myPlayers[WHITE]->stop();
	myPlayers[BLACK]->stop();
}

void Game::setBoardView(GuiBoardView* boardView) {
	myBoardView = boardView;
}

void Game::setHistoryView(GuiHistoryView* historyView) {
	myHistoryView = historyView;
}

void Game::setPlayer(int color, Player* player) {
	myPlayers[color] = player;
}


void Game::setModel(Model* model) {
	myModel = model;
	return;
}


void Game::moveReady(const Move& move) {

	qDebug() << ":Game: "<< "got  move: " << move.pos1.myX << move.pos1.myY
								          << move.pos2.myX << move.pos2.myY;

	if (move.player != myCurrentPlayer) {
		return;
	}

	if (myModel->canMove(move) == false) {
		return;
	}

	myModel->makeMove(move);
	qDebug() << ":Game: "<< " move made: ";

	myBoardView->update();
	myCurrentPlayer = 1 - myCurrentPlayer;

	GameStatus status = myModel->checkGameStatus(myCurrentPlayer);
	if (status == MATE) {
		QString playerName = myCurrentPlayer == WHITE ? "white" : "black";
		QMessageBox("game message", QString("player ")+playerName+" has got the mate!",
				    QMessageBox::Information, QMessageBox::Ok,0,0).exec();
	} else if (status == STALEMATE) {
		QMessageBox("game message", "It's the stalemate!",
					QMessageBox::Information, QMessageBox::Ok,0,0).exec();
	} else {
		myPlayers[myCurrentPlayer]->makeTurn();
	}

}
void Game::changeModel(const Model& model) {
//	qDebug() << ":Game: "<< "changeModel!";
//	*myModel = model;
//	myModel->addListener(myBoardView);
//	myModel->addListener(myHistoryView);
//	myBoardView->turnMaked(Move());
}
void Game::init() {

	assert(myModel != 0);
	assert(myBoardView != 0);
	assert(myHistoryView != 0);
	assert(myPlayers[WHITE] != 0);
	assert(myPlayers[BLACK] != 0);

	myBoardView->setModel(myModel);

	myHistoryView->setModel(myModel);
	myHistoryView->addListener(this);
	myHistoryView->setBoardView(myBoardView);

	myModel->addListener(myBoardView);
	myModel->addListener(myHistoryView);

	for (int i = 0; i < 2; ++i) {
		myPlayers[i]->addListener(this);
		myBoardView->addListener(myPlayers[i]);
	}



	myCurrentPlayer = myModel->getFirstTurnPlayer();

	myPlayers[myCurrentPlayer]->makeTurn();

	myBoardView->updateIt();

}

void Game::start() {
	myPlayers[myCurrentPlayer]->makeTurn();
}
