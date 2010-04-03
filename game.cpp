/*
 * game.cpp
 *
 *
 *      Author: snowwlex
 */

#include <ncurses.h>
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
#include "view.h"
#include "game.h"
#include "player.h"
#include "ai.h"
#include "history.h"

#include "experiment.h"
#include "exprunner.h"

struct foo {
	void operator()(int number) {
		for (int i=0; i < 500*number; ++i) {
			DEBUG_DISPLAY("asdsad")
		}
	}
};

class aiTester {
	private:
		AIPlayer* myPlayer;
	public:
	aiTester(AIPlayer* player) : myPlayer(player) { }
	void operator()(int number) {
		Move move;
		myPlayer->setDepth(number);
		myPlayer->makeTurn(move,NONE);
	}
};

PlayerColor Game::start(std::string file, int mode, playerInfo onePlayer, playerInfo twoPlayer) {



	Rules rules;
	Model model(&rules);
	RulesIO rules_io(&rules);
	ModelIO model_io(&model);
	History history;

	if ( mode == 0) {
		file = std::string("rules/") + file + std::string(".xml");
		rules_io.load(file);
		rules_io.updateRules();
		model.init(0);
	}
	else {
		file = std::string("saves/") + file + std::string(".xml");
		model_io.load(file);
		file = std::string("rules/") + model_io.getStorage().rulesName + std::string(".xml");
		rules_io.load(file);
		rules_io.updateRules();
		model_io.updateModel();
		model.init(1);
	}

	CLIView* infoView = new CLIView(25, 30, 14,2,6, true);
	BoardCLIView *boardView = new BoardCLIView(model.getBoardSizeX()+2,model.getBoardSizeY()+10,2,2,7, false, &model);
	CLIView* userView[2];
	userView[WHITE] = new CLIView(10,37,2,35,8,true);
	userView[BLACK] = new CLIView(10,37,13,35,9,true);
	CLIView* askView;

	Player *players[2];

	if (onePlayer.playerName == "Human") {
		players[WHITE] = new HumanPlayer(WHITE, &model, boardView, userView[WHITE]);
	} else if (onePlayer.playerName == "Parallel") {
		players[WHITE] = new AlphaBetaParallelSearchAIPlayer(WHITE, &model, boardView, userView[WHITE],onePlayer.num1,onePlayer.num2);
	} else if (onePlayer.playerName == "FullSearch") {
		players[WHITE] = new FullSearchAIPlayer(WHITE, &model, boardView, userView[WHITE],onePlayer.num1,onePlayer.num2);
	} else  {
		players[WHITE] = new AlphaBetaSearchAIPlayer(WHITE, &model, boardView, userView[WHITE],onePlayer.num1,onePlayer.num2);
	}


	if (twoPlayer.playerName == "Human") {
		players[BLACK] = new HumanPlayer(BLACK, &model, boardView, userView[BLACK]);
	} else if (twoPlayer.playerName == "Parallel") {
		players[BLACK] = new AlphaBetaParallelSearchAIPlayer(BLACK, &model, boardView, userView[BLACK],twoPlayer.num1,twoPlayer.num2);
	} else if (twoPlayer.playerName == "FullSearch") {
		players[BLACK] = new FullSearchAIPlayer(BLACK, &model, boardView, userView[BLACK],twoPlayer.num1,twoPlayer.num2);
	} else  {
		players[BLACK] = new AlphaBetaSearchAIPlayer(BLACK, &model, boardView, userView[BLACK],twoPlayer.num1,twoPlayer.num2);
	}




	// EXPERIMENT
//	Experiment exp(0.6,3,10);
//	ExpRunner exprunner(exp);
//	int n[] = { 1,2,3 };
//	exprunner.run(aiTester((AIPlayer*)players[BLACK]), std::vector<int>(n,n+3), "gnuplot.txt");


	int curPlayer;
	bool isEndGame, isMoveCorrect;
	char buffer[1024];
	std::string string;
	GameMessage message;
	PlayerCommand command;
	Move playerMove;
	GameStatus status;
	message = NONE;


	sprintf(buffer,"Hello) Use arrows to\nselect figure,\nthen SPACE - choose figure\nthen select destination cell!\nGood luck)\n"); infoView->render(buffer);
	do {

		isEndGame = false;

		curPlayer = model.getCurrentPlayer();

		boardView->render();
		status = model.getGameStatus(curPlayer);
		switch(status) {
		case CHECK:
			message = GOT_CHECK;
			break;
		case MATE:
			isEndGame = true;
			sprintf(buffer,"Player %s has got the MATE!\n", model.getPlayerData(curPlayer).c_str());
			fputs(buffer, statfile);
			infoView->render(buffer);
			infoView->wait();
			break;
		case STALEMATE:
			isEndGame = true;
			sprintf(buffer,"Player %s has got the STALEMATE!\n", model.getPlayerData(curPlayer).c_str());
			fputs(buffer, statfile);
			infoView->render(buffer);
			infoView->wait();
			break;
		default:
			break;
		}

		if (isEndGame == false) {
			do {

				command = players[curPlayer]->makeTurn(playerMove, message);
				if (command == TURN) {
					isMoveCorrect = model.canMove(playerMove);
					if ( isMoveCorrect == false) {
						message = WRONG_MOVE;
					}
				}
			}
			while( command == TURN && isMoveCorrect == false );

			message = NONE;
			switch(command) {
				case TURN:
					model.makeMove(playerMove);
					model.setCurrentPlayer( 1 - curPlayer );
					history.addRecord(playerMove, model);
					break;
				case EXIT:
					isEndGame = true;
					break;
				case SAVE:
					askView = new CLIView(5,30,25,35,6);
					askView->render("Enter savename:\n");
					string = askView->ask("> ");
					delete askView;
					string = std::string("saves/") + string + std::string(".xml");
					model_io.save(string);
					message = SAVED;
					break;
				case UNDO:
					model = history.makeUndo(2);
					break;
				default:
					break;
			}
		}
	} while( isEndGame == false );


	delete players[WHITE];
	delete players[BLACK];

	delete infoView;
	delete boardView;
	delete userView[WHITE];
	delete userView[BLACK];

	PlayerColor whoWin;
	if (status == STALEMATE) {
		whoWin = ALL;
	} else {
		if (model.getCurrentPlayer() == WHITE) {
			whoWin = BLACK;
		} else {
			whoWin = WHITE;
		}
	}
	return whoWin;
}
