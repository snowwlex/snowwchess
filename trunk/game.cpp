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

	View *info_view = new View(30, 60, 2,80,6, true);
	View *board_view = new BoardCLIView(10,13,2,2,7, false, &model);
	View *user_view[2];
	user_view[WHITE] = new View(10,60,17,1,8,true);
	user_view[BLACK] = new View(10,60,17,1,9,true);

	Player *players[2];
	players[WHITE] = new HumanPlayer(WHITE, &model, board_view, user_view[WHITE]);
	players[BLACK] = new HumanPlayer(BLACK, &model, board_view, user_view[BLACK]);

	int cur_player = WHITE;
	bool is_end = false, correct_move;
	GameMessage message;
	PlayerCommand command;
	Move player_move;
	message = NONE;
	do {
		correct_move = false;
		do {
			command = players[cur_player]->YourTurn(player_move, message);
			if (player_move.pos1.x == 0 && player_move.pos2.y == 0) {
				correct_move = true;
				is_end = true;
			}	else if ( !model.CanMove(player_move) ) {
				message = WRONG_MOVE;
			} else {
				correct_move = true;
			}
		}
		while( !correct_move );
		if (!is_end) model.Remove(player_move);
		if ( !is_end && model.IsWin(cur_player) == true) {
			is_end = true;
			std::string msg;
			msg = "Player ";
			msg += (cur_player == WHITE ? "BLACK" : "WHITE");
			msg += " has got the mate!";
			info_view->Render(msg);
		}

		cur_player = (cur_player == WHITE ? BLACK : WHITE);
		if ( !is_end && model.IsCheck(cur_player) ) {
			message = CHECK;
		}
		else {
			message = NONE;
		}
	} while( !is_end );


	delete players[WHITE];
	delete players[BLACK];

	delete info_view;
	delete board_view;
	delete user_view[WHITE];
	delete user_view[BLACK];
}
