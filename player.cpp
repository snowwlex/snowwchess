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



HumanPlayer::HumanPlayer() {

}

Move HumanPlayer::makeTurn(GameMessage message) {

	PlayerCommand command;
	std::string inputCommand;
	MOVES moves;
	Move move;
	MOVES::iterator itMove;
	int key,mode;
	switch(message) {
		case WRONG_MOVE:
				qDebug() << "Received message: Wrong move!\n";
				break;
		case GOT_CHECK:
				qDebug() << "Received message: You've gotta check!";
				break;
		default:
				break;
	}


	//myBoardView->render();
	//myBoardView->highlight(myCursorPos,13);

	command = NOTHING;
	mode = 0;
	do {


		//key = myUserView->getKey();

		//myBoardView->render();

		switch(key) {
			case KEY_UP:
			//	if (myCursorPos.myY > 0) myCursorPos.myY -= 1;
				break;
			case KEY_DOWN:
			//	if (myCursorPos.myY < myModel->getBoardSizeY()-1 ) myCursorPos.myY += 1;
				break;
			case KEY_LEFT:
			//	if (myCursorPos.myX > 0 ) myCursorPos.myX -= 1;
				break;
			case KEY_RIGHT:
			//	if (myCursorPos.myX < myModel->getBoardSizeX()-1 ) myCursorPos.myX += 1;
				break;
			case 27:
				command = EXIT;
				break;
			case KEY_F(5):
				command = SAVE;
				break;
			case KEY_F(9):
				command = UNDO;
				break;
			case KEY_F(4):
				moves = myModel->allMoves(myColor);
				for ( itMove=moves.begin() ; itMove != moves.end(); itMove++ ) {
					//myBoardView->highlight(itMove->pos2,(itMove->type & MOVE) ? 12 : 11);
				}
				break;
			case ' ':
				if (mode == 0) {
					//move.pos1.myX = myCursorPos.myX;
					//move.pos1.myY = myCursorPos.myY;
					moves = myModel->movesFromPosition(myColor, Position(move.pos1.myX,move.pos1.myY));
					mode = 1;
				//} else if (myCursorPos.myX == move.pos1.myX && myCursorPos.myY == move.pos1.myY){
					mode = 0;
				} else {
					//move.pos2.myX = myCursorPos.myX;
					//move.pos2.myY = myCursorPos.myY;
					move.player = myColor;
					move.type = CAPTURE | MOVE;
					command = TURN;
				}
				break;
			default:
				break;
		}


		if (mode == 1) {
			for ( itMove=moves.begin() ; itMove != moves.end(); ++itMove ) {
				//myBoardView->highlight(itMove->pos2,(itMove->type & MOVE) ? 12 : 11);
			}
			//myBoardView->highlight(Position(move.pos1.myX,move.pos1.myY),13);
		}
		//myBoardView->highlight(myCursorPos,13);




	} while( command == NOTHING );


	return move;
}


