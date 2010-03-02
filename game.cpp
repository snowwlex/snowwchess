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
#include "rules.h"
#include "model.h"
#include "io.h"
#include "view.h"
#include "game.h"
#include "player.h"


void Game::Start() {

	Rules rules;
	Model model(&rules);
	RulesIO rules_io(&rules);
	ModelIO model_io(&model);
	rules_io.Load();
	model_io.Load();

	CLIView *info_view = new CLIView(25, 30, 14,2,6, true);
	CLIView *board_view = new BoardCLIView(12,20,2,2,7, false, &model);
	CLIView *user_view[2];
	user_view[WHITE] = new CLIView(10,70,2,40,8,true);
	user_view[BLACK] = new CLIView(10,70,13,40,9,true);

	Player *players[2];
	players[WHITE] = new HumanPlayer(WHITE, &model, board_view, user_view[WHITE]);
	players[BLACK] = new HumanPlayer(BLACK, &model, board_view, user_view[BLACK]);

	int cur_player = WHITE;
	bool isEndGame, correct_move;
	char buffer[1024];
	GameMessage message;
	PlayerCommand command;
	Move player_move;
	GameStatus status;

	do {
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
			message = NONE;
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


			switch(command) {
				case TURN:
					model.Remove(player_move);

					cur_player = (cur_player == WHITE ? BLACK : WHITE);
					break;
				case EXIT:
					isEndGame = true;
					break;
				case SAVE:
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
