/*
 * game.cpp
 *
 *
 *      Author: snowwlex
 */


//#include <ncurses.h> //ncurses is temporaly not supported;

#include <QMessageBox>

#include <vector>
#include <string>
#include <map>
#include <deque>
#include <algorithm>

#include <ctime>

#include "snowwchess.h"
#include "model.h"
#include "rules.h"
#include "rules_io.h"
#include "model_io.h"
//#include "view.h"
#include "guiboardview.h"
#include "player.h"
#include "ai.h"
#include "game.h"


#include "history.h"

#include "experiment.h"
#include "exprunner.h"
#include "tester.h"

Game::Game() {
	players[WHITE] = players[BLACK] = 0;
	myBoardView = 0;
}

Game::~Game() {
	this->clear();
}

void Game::loadRules(const RulesIO& rulesIO) {
	rulesIO.updateRules(myRules);
}

void Game::setBoardView(GuiBoardView* boardView) {
	myBoardView = boardView;
}

void Game::setPlayer(int color, Player* player) {
	players[color] = player;
}

void Game::prepare() {
	qDebug() << "init model";
	myModel.setRules(&myRules);
	myModel.init(true); // true is set temporaly; set because of only 'new game' mode is avaliable now;
}
bool Game::isReady() {
	if ( players[WHITE] != 0 &&
		 players[BLACK] != 0 &&
		 myBoardView    != 0 &&
		 myModel.isReady()
	   )
	{
		qDebug() << "returning true";
		return true;
	}
	return false;
}


void Game::clear() {
	delete players[WHITE];
	delete players[BLACK];
}


void Game::moveReady(const Move& move) {

	qDebug() << ":Game: "<< "got  move: " << move.pos1.myX << move.pos1.myY
								          << move.pos2.myX << move.pos2.myY;

	if (move.player != curPlayer) {
		return;
	}

	if (myModel.canMove(move) == false) {
		return;
	}

	myModel.makeMove(move);
	qDebug() << ":Game: "<< " move made: ";

	myBoardView->update();
	curPlayer = 1 - curPlayer;

	GameStatus status = myModel.checkGameStatus(curPlayer);
	if (status == MATE) {
		QString playerName = curPlayer == WHITE ? "white" : "black";
		QMessageBox("game message", QString("player ")+playerName+" has got the mate!",
				    QMessageBox::Information, QMessageBox::Ok,0,0).exec();
	} else if (status == STALEMATE) {
		QMessageBox("game message", "It's the stalemate!",
					QMessageBox::Information, QMessageBox::Ok,0,0).exec();
	} else {
		players[curPlayer]->makeTurn();
	}

}

void Game::start() {


	myBoardView->setModel(&myModel);
	myModel.addListener(myBoardView);
	for (int i = 0; i < 2; ++i) {
		players[i]->setModel(&myModel);
		players[i]->setColor(i);
		players[i]->setBoardView(myBoardView);

		players[i]->addListener(this);
		myBoardView->addListener(players[i]);
	}

	curPlayer = myModel.getCurrentPlayer();

	players[curPlayer]->makeTurn();

	myBoardView->update();

}
