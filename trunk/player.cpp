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


HumanPlayer::HumanPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView): myColor(color), myModel(model), myBoardView(boardView), myUserView(userView) { myCursorPos.myX = 0; myCursorPos.myY=0;}

PlayerCommand HumanPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	std::string inputCommand;
	std::vector< Move > moves;
	std::vector < Move >::iterator itMove;
	int key,mode;
	switch(message) {
		case WRONG_MOVE:
				myUserView->render("Wrong move!\n");
				break;
		case GOT_CHECK:
				myUserView->render("You've gotta check!\n");
				break;
		case SAVED:
				myUserView->render("Game saved\n");
				break;
		default:
				break;
	}


	myBoardView->render();
	myBoardView->highlight(myCursorPos,13);

	command = NOTHING;
	mode = 0;
	do {


		key = myUserView->getKey();

		myBoardView->render();

		switch(key) {
			case KEY_UP:
				if (myCursorPos.myY > 0) myCursorPos.myY -= 1;
				break;
			case KEY_DOWN:
				if (myCursorPos.myY < myModel->getBoardSizeY()-1 ) myCursorPos.myY += 1;
				break;
			case KEY_LEFT:
				if (myCursorPos.myX > 0 ) myCursorPos.myX -= 1;
				break;
			case KEY_RIGHT:
				if (myCursorPos.myX < myModel->getBoardSizeX()-1 ) myCursorPos.myX += 1;
				break;
			case 27:
				command = EXIT;
				break;
			case KEY_F(5):
				command = SAVE;
				break;
			case KEY_F(4):
				moves = myModel->moves(myColor);
				for ( itMove=moves.begin() ; itMove != moves.end(); itMove++ ) {
					myBoardView->highlight(itMove->pos2,(itMove->type & MOVE) ? 12 : 11);
				}
				break;
			case ' ':
				if (mode == 0) {
					move.pos1.myX = myCursorPos.myX;
					move.pos1.myY = myCursorPos.myY;
					moves = myModel->moves(myColor, Position(move.pos1.myX,move.pos1.myY));
					mode = 1;
				} else if (myCursorPos.myX == move.pos1.myX && myCursorPos.myY == move.pos1.myY){
					mode = 0;
				} else {
					move.pos2.myX = myCursorPos.myX;
					move.pos2.myY = myCursorPos.myY;
					move.player = myColor;
					move.type = CAPTURE | MOVE;
					move.figureId = myModel->findFigure(myColor,move.pos1)->id;
					command = TURN;
				}
				break;
			default:
				break;
		}


		if (mode == 1) {
			for ( itMove=moves.begin() ; itMove != moves.end(); ++itMove ) {
			myBoardView->highlight(itMove->pos2,(itMove->type & CAPTURE) ? 11 : 12);
			}
			myBoardView->highlight(Position(move.pos1.myX,move.pos1.myY),13);
		}
		myBoardView->highlight(myCursorPos,13);




	} while( command == NOTHING );


	return command;
}



HumanAltPlayer::HumanAltPlayer(int _color, Model* m, BoardCLIView *board_view, CLIView * user_view): myColor(_color), myModel(m), myBoardView(board_view), myUserView(user_view) { }

PlayerCommand HumanAltPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	std::string inputCommand;
	int x1,y1, x2,y2;
	std::vector< Move > moves;
	char buffer[1024];
	curs_set(1);

	switch(message) {
		case WRONG_MOVE:
				myUserView->render("Wrong move!\n");
				break;
		case GOT_CHECK:
				myUserView->render("You've gotta check!\n");
				break;
		case SAVED:
				myUserView->render("Game saved\n");
				break;
		default:
				break;
	}

	myBoardView->render();

	command = NOTHING;
	do {

		inputCommand = myUserView->ask("> ");
		if (inputCommand.length() == 2) {

			x1 = inputCommand[0]-'a';
			y1 = myModel->getBoardSizeY()-inputCommand[1]+'0';

			moves.clear();

			moves = myModel->moves(myColor, Position(x1,y1));
			sprintf(buffer, "Available moves for %c%c:\n", x1+'a',myModel->getBoardSizeY()-y1+'0');
			myUserView->render(buffer);
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it != moves.end(); it++ ) {
				sprintf(buffer, "[%c%c-%c%c (%s)] ", x1+'a',myModel->getBoardSizeY() - y1+'0',it->pos2.myX+'a', myModel->getBoardSizeY() - it->pos2.myY+'0', it->type == CAPTURE ? "eat" : "move" );
				myUserView->render(buffer);
			}
			myUserView->render("\n");
		} else if (inputCommand == "moves") {

			moves.clear();

			moves = myModel->moves(myColor);
			sprintf(buffer, "\n");
			myUserView->render("Available moves for all\n");
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it != moves.end(); it++ ) {
				sprintf(buffer, "[%c%c-%c%c (%s)] ", it->pos1.myX+'a',myModel->getBoardSizeY() - it->pos1.myY+'0',it->pos2.myX+'a', myModel->getBoardSizeY() - it->pos2.myY+'0', it->type == CAPTURE ? "eat" : "move" );
				myUserView->render(buffer);
			}
			myUserView->render("\n");
		} else if (inputCommand == "exit") {
			command = EXIT;
		} else if (inputCommand == "save") {
					command = SAVE;
		} else {
			x1 = inputCommand[0]-'a';
			y1 = myModel->getBoardSizeY() - inputCommand[1] + '0';
			x2 = inputCommand[3]-'a';
			y2 = myModel->getBoardSizeY() - inputCommand[4] + '0';
			move.pos1.myX = x1;
			move.pos1.myY = y1;
			move.pos2.myX = x2;
			move.pos2.myY = y2;
			move.player = myColor;
			move.type = CAPTURE | MOVE;
			command = TURN;
		}

	} while( command == NOTHING );

	return command;
}
