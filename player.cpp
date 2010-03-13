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
	MOVES moves;
	MOVES::iterator itMove;
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
			myBoardView->highlight(itMove->pos2,(itMove->type & MOVE) ? 12 : 11);
			}
			myBoardView->highlight(Position(move.pos1.myX,move.pos1.myY),13);
		}
		myBoardView->highlight(myCursorPos,13);




	} while( command == NOTHING );


	return command;
}


