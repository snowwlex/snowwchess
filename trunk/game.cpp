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


void Game::Start(std::string file, int mode) {

	Rules rules;
	Model model(&rules);
	RulesIO rules_io(&rules);
	ModelIO model_io(&model);

	if ( mode == 0) {
		file = std::string("rules/") + file + std::string(".xml");
		rules_io.Load(file);
		rules_io.UpdateRules();
		model.Init(0);
	}
	else {
		file = std::string("saves/") + file + std::string(".xml");
		model_io.Load(file);
		file = std::string("rules/") + model_io.getStorage().rules_name + std::string(".xml");
		rules_io.Load(file);
		rules_io.UpdateRules();
		model_io.UpdateModel();
		model.Init(1);
	}

	CLIView *choose_view;
	CLIView *info_view = new CLIView(25, 30, 14,2,6, true);
	BoardCLIView *board_view = new BoardCLIView(model.GetBoardSizeX()+2,model.GetBoardSizeY()+10,2,2,7, false, &model);
	CLIView *user_view[2];
	user_view[WHITE] = new CLIView(10,70,2,40,8,true);
	user_view[BLACK] = new CLIView(10,70,13,40,9,true);


	Player *players[2];
	players[WHITE] = new HumanPlayer(WHITE, &model, board_view, user_view[WHITE]);
	players[BLACK] = new HumanPlayer(BLACK, &model, board_view, user_view[BLACK]);

	int cur_player;
	bool isEndGame, correct_move;
	char buffer[1024];
	std::string string;
	GameMessage message;
	PlayerCommand command;
	Move player_move;
	GameStatus status;
	message = NONE;
	do {
		cur_player = model.GetCurrentPlayer();
		isEndGame = false;

		status = model.GetGameStatus(cur_player);
		switch(status) {
		case CHECK:
			message = GOT_CHECK;
			break;
		case MATE:
			isEndGame = true;
			sprintf(buffer,"Player %s has got the MATE!", cur_player == WHITE ? "BLACK" : "WHITE");
			info_view->Render(buffer);
			info_view->Ask();
			break;
		case STALEMATE:
			isEndGame = true;
			sprintf(buffer,"Player %s has got the STALEMATE!", cur_player == WHITE ? "BLACK" : "WHITE");
			info_view->Render(buffer);
			info_view->Ask();
			break;
		default:
			break;
		}

		if (isEndGame == false) {
			do {
				command = players[cur_player]->YourTurn(player_move, message);
				if (command == TURN) {
					if (player_move.pos1.x == 0 && player_move.pos2.y == 0) {
						correct_move = true;
					}	else if ( model.CanMove(player_move) == false) {
						message = WRONG_MOVE;
						correct_move = false;
					} else {
						correct_move = true;
					}
				}
			}
			while( command == TURN && correct_move == false );

			message = NONE;
			switch(command) {
				case TURN:
					model.Remove(player_move);
					model.SetCurrentPlayer( cur_player == WHITE ? BLACK : WHITE );
					break;
				case EXIT:
					isEndGame = true;
					break;
				case SAVE:
					choose_view = new CLIView(5,30,25,35,6);
					choose_view->Render("Enter savename:\n");
					string = choose_view->Ask("> ");
					delete choose_view;
					string = std::string("saves/") + string + std::string(".xml");
					model_io.Save(string);
					message = SAVED;
					break;
				default:
					break;
			}
		}
	} while( isEndGame == false );


	delete players[WHITE];
	delete players[BLACK];

	delete info_view;
	delete board_view;
	delete user_view[WHITE];
	delete user_view[BLACK];
}
