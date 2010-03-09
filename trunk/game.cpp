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

#include "snowwchess.h"
#include "model.h"
#include "rules.h"
#include "rules_io.h"
#include "model_io.h"
#include "view.h"
#include "game.h"
#include "player.h"
#include "ai.h"


void Game::start(std::string file, int mode) {

	Rules rules;
	Model model(&rules);
	RulesIO rules_io(&rules);
	ModelIO model_io(&model);

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

	CLIView *infoView = new CLIView(25, 30, 14,2,6, true);
	BoardCLIView *boardView = new BoardCLIView(model.getBoardSizeX()+2,model.getBoardSizeY()+10,2,2,7, false, &model);
	CLIView *userView[2];
	userView[WHITE] = new CLIView(10,28,2,40,8,true);
	userView[BLACK] = new CLIView(10,28,13,40,9,true);


	Player *players[2];
	//players[WHITE] = new HumanPlayer(WHITE, &model, boardView, userView[WHITE]);
	players[WHITE] = new AIPlayer(WHITE, &model, boardView, userView[WHITE]);
	players[BLACK] = new AIPlayer(BLACK, &model, boardView, userView[BLACK]);

	int curPlayer;
	int key;
	bool isEndGame, isMoveCorrect;
	char buffer[1024];
	std::string string;
	GameMessage message;
	PlayerCommand command;
	Move playerMove;
	GameStatus status;
	message = NONE;
	do {

		isEndGame = false;

		key = boardView->getKey();
		if (key == KEY_F(5) ) {
			infoView = new CLIView(5,30,25,35,6);
			infoView->render("Enter savename:\n");
			string = infoView->ask("> ");
			delete infoView;
			string = std::string("saves/") + string + std::string(".xml");
			model_io.save(string);
			message = SAVED;
		} else if (key == 27) {
			isEndGame = true;
		}
		curPlayer = model.getCurrentPlayer();

		boardView->render();
		sprintf(buffer,"start() getting game status\n"); debugView->render(buffer);
		status = model.getGameStatus(curPlayer);
		sprintf(buffer,"start() game status received\n\n"); debugView->render(buffer);
		switch(status) {
		case CHECK:
			message = GOT_CHECK;
			break;
		case MATE:
			isEndGame = true;
			sprintf(buffer,"Player %s has got the MATE!", model.getPlayerData(curPlayer).c_str());
			infoView->render(buffer);
			infoView->ask();
			break;
		case STALEMATE:
			isEndGame = true;
			sprintf(buffer,"Player %s has got the STALEMATE!", model.getPlayerData(curPlayer).c_str());
			infoView->render(buffer);
			infoView->ask();
			break;
		default:
			break;
		}

		if (isEndGame == false) {
			do {
				command = players[curPlayer]->makeTurn(playerMove, message);
				if (command == TURN) {
					sprintf(buffer,"start() check if move is correct\n"); debugView->render(buffer);
					isMoveCorrect = model.canMove(playerMove);
					sprintf(buffer,"start() end of check; move is %scorrect\n\n",isMoveCorrect?"":"not "); debugView->render(buffer);
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
					break;
				case EXIT:
					isEndGame = true;
					break;
				case SAVE:
					infoView = new CLIView(5,30,25,35,6);
					infoView->render("Enter savename:\n");
					string = infoView->ask("> ");
					delete infoView;
					string = std::string("saves/") + string + std::string(".xml");
					model_io.save(string);
					message = SAVED;
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
}
