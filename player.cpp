/*
 * player.cpp
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
#include "view.h"
#include "player.h"


HumanPlayer::HumanPlayer(int _color, Model* m, BoardCLIView *board_view, CLIView * user_view): myColor(_color), model(m), myBoardView(board_view), myUserView(user_view) { cursorPos.x = 0; cursorPos.y=0;}

PlayerCommand HumanPlayer::YourTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	std::string input_command;
	std::vector< Move > moves;
	std::vector < Move >::iterator it;
	int key,mode;
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
	myBoardView->Highlight(cursorPos,13);

	command = NOTHING;
	mode = 0;
	do {


		key = myUserView->GetKey();

		myBoardView->Render();

		switch(key) {
			case KEY_UP:
				if (cursorPos.y > 0) cursorPos.y -= 1;
				break;
			case KEY_DOWN:
				if (cursorPos.y < model->GetBoardSizeY()-1 ) cursorPos.y += 1;
				break;
			case KEY_LEFT:
				if (cursorPos.x > 0 ) cursorPos.x -= 1;
				break;
			case KEY_RIGHT:
				if (cursorPos.x < model->GetBoardSizeX()-1 ) cursorPos.x += 1;
				break;
			case 27:
				command = EXIT;
				break;
			case KEY_F(5):
				command = SAVE;
				break;
			case KEY_F(4):
				moves = model->Moves(myColor);
				for ( it=moves.begin() ; it != moves.end(); it++ ) {
					myBoardView->Highlight(it->pos2,(it->type & EAT) ? 11 : 12);
				}
				break;
			case ' ':
				if (mode == 0) {
					move.pos1.x = cursorPos.x;
					move.pos1.y = cursorPos.y;
					moves = model->Moves(myColor, Position(move.pos1.x,move.pos1.y));
					mode = 1;
				} else if (cursorPos.x == move.pos1.x && cursorPos.y == move.pos1.y){
					mode = 0;
				} else {
					move.pos2.x = cursorPos.x;
					move.pos2.y = cursorPos.y;
					move.player = myColor;
					move.type = EAT | MOVE;
					move.figure_id = model->findFigure(myColor,move.pos1)->id;
					command = TURN;
				}
				break;
			default:
				break;
		}


		if (mode == 1) {
			for ( it=moves.begin() ; it != moves.end(); ++it ) {
			myBoardView->Highlight(it->pos2,(it->type & EAT) ? 11 : 12);
			}
			myBoardView->Highlight(Position(move.pos1.x,move.pos1.y),13);
		}
		myBoardView->Highlight(cursorPos,13);




	} while( command == NOTHING );


	return command;
}



HumanAltPlayer::HumanAltPlayer(int _color, Model* m, BoardCLIView *board_view, CLIView * user_view): myColor(_color), model(m), myBoardView(board_view), myUserView(user_view) { }

PlayerCommand HumanAltPlayer::YourTurn(Move& move, GameMessage message) {

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
			move.type = EAT | MOVE;
			command = TURN;
		}

	} while( command == NOTHING );

	return command;
}
