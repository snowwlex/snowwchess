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

HumanPlayer::HumanPlayer(int _color, Model* m, CLIView *board_view, CLIView * user_view): myColor(_color), model(m), myBoardView(board_view), myUserView(user_view) { }

PlayerCommand HumanPlayer::YourTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	std::string input_command;
	int x1,y1, x2,y2;
	std::vector< Move > moves;
	char buffer[1024];
	curs_set(1);

	switch(message) {
		case WRONG_MOVE:
				myUserView->Render("Wrong move!\n");
				break;
		case GOT_CHECK:
				myUserView->Render("You've gotta check!\n");
				break;
		case SAVED:
				myUserView->Render("Game saved\n");
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
			y1 = model->GetBoardSizeY()-input_command[1]+'0';

			moves.clear();

			moves = model->Moves(myColor, Position(x1,y1));
			sprintf(buffer, "Available moves for %c%c:\n", x1+'a',model->GetBoardSizeY()-y1+'0');
			myUserView->Render(buffer);
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it != moves.end(); it++ ) {
				sprintf(buffer, "[%c%c-%c%c (%s)] ", x1+'a',model->GetBoardSizeY() - y1+'0',it->pos2.x+'a', model->GetBoardSizeY() - it->pos2.y+'0', it->type == EAT ? "eat" : "move" );
				myUserView->Render(buffer);
			}
			myUserView->Render("\n");
		} else if (input_command == "moves") {

			moves.clear();

			moves = model->Moves(myColor);
			sprintf(buffer, "\n");
			myUserView->Render("Available moves for all\n");
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it != moves.end(); it++ ) {
				sprintf(buffer, "[%c%c-%c%c (%s)] ", it->pos1.x+'a',model->GetBoardSizeY() - it->pos1.y+'0',it->pos2.x+'a', model->GetBoardSizeY() - it->pos2.y+'0', it->type == EAT ? "eat" : "move" );
				myUserView->Render(buffer);
			}
			myUserView->Render("\n");
		} else if (input_command == "exit") {
			command = EXIT;
		} else if (input_command == "save") {
					command = SAVE;
		} else {
			x1 = input_command[0]-'a';
			y1 = model->GetBoardSizeY() - input_command[1] + '0';
			x2 = input_command[3]-'a';
			y2 = model->GetBoardSizeY() - input_command[4] + '0';
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
