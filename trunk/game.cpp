/*
 * game.cpp
 *
 *
 *      Author: snowwlex
 */


//#include <ncurses.h> //ncurses is temporaly not supported;

#include <vector>
#include <string>
#include <map>
#include <deque>

#include <ctime>

#include "snowwchess.h"
#include "model.h"
#include "rules.h"
#include "rules_io.h"
#include "model_io.h"
//#include "view.h"
#include "player.h"
#include "ai.h"
#include "guiboardview.h"
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

void Game::setRules(const RulesIO& rulesIO) {
	rulesIO.updateRules(myRules);
}

void Game::setBoardView(GuiBoardView* boardView) {
	myBoardView = boardView;
}

void Game::setPlayer(int color, Player* player) {
	players[color] = player;
}

bool Game::isReady() {
	myModel.init(true); // true is set temporaly; set because of only 'new game' mode is avaliable now;

	if ( players[WHITE] != 0 &&
		 players[BLACK] != 0 &&
		 myModel.isReady()   &&
		 myBoardView    != 0
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

void Game::start() {

	History history;




}
