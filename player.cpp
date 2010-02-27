/*
 * player.cpp
 *
 *
 *      Author: snowwlex
 */

#include <ncurses.h>
#include "vector"
#include "string"
#include "map"
#include "rules.h"
#include "model.h"
#include "view.h"
#include "game.h"
#include "player.h"

HumanPlayer::HumanPlayer(int _color, Model* m, View *board_view, View * user_view): myColor(_color), model(m), myBoardView(board_view), myUserView(user_view) { }

PlayerCommand HumanPlayer::YourTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	std::string input_command;
	int x1,y1, x2,y2;
	std::vector< Move > moves;
	curs_set(1);

	switch(message) {
		case WRONG_MOVE:
				myUserView->Render("Wrong move!");
				break;
		case CHECK:
				myUserView->Render("You've gotta check!");
				break;
		default:
				break;
	}

	myBoardView->Render();

	command = NOTHING;
	do {

		input_command = myUserView->Ask("> ");
		if (input_command.length() == 2) {
			x1 = input_command[0]-'a';
			y1 = 8-input_command[1]+'0';

			moves.clear();

			moves = model->Moves(myColor, Position(x1,y1));
			wprintw(myUserView->myWindow, "Available moves for %c%c:\n", x1+'a',8-y1+'0');
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it != moves.end(); it++ ) {
				wprintw(myUserView->myWindow, "[%c%c-%c%c (%s)] ", x1+'a',8 - y1+'0',it->pos2.x+'a', 8 - it->pos2.y+'0', it->type == EAT ? "eat" : "move" );
			}
			wprintw(myUserView->myWindow, "\n");
			wrefresh(myUserView->myWindow);
		} else if (input_command == "moves") {

			moves.clear();

			moves = model->Moves(myColor);
			wprintw(myUserView->myWindow, "Available moves for all\n", x1+'a',8-y1+'0');
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it != moves.end(); it++ ) {
				wprintw(myUserView->myWindow, "[%c%c-%c%c (%s)] ", it->pos1.x+'a',8 - it->pos1.y+'0',it->pos2.x+'a', 8 - it->pos2.y+'0', it->type == EAT ? "eat" : "move" );
			}
			wprintw(myUserView->myWindow, "\n");
			wrefresh(myUserView->myWindow);
		} else if (input_command == "exit") {
			command = EXIT;

		} else {
			x1 = input_command[0]-'a';
			y1 = 8 - input_command[1] + '0';
			x2 = input_command[3]-'a';
			y2 = 8 - input_command[4] + '0';
			move.pos1.x = x1;
			move.pos1.y = y1;
			move.pos2.x = x2;
			move.pos2.y = y2;
			move.player = myColor;
			move.type = EAT_MOVE;
			command = TURN;
		}

	} while( command == NOTHING );

	return command;
}
