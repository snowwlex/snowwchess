/*
 *  *
 *
 *      Author: snowwlex
 */

#include "human_player.h"

HumanPlayer::HumanPlayer() {
	myBoardView = 0;
	iAmMoving = false;
	catchDestination = false;
}

void HumanPlayer::pushedCell(const Position& pos) {

	qDebug() << ":HumanPlayer:" << "get pushed cell, my color is" << myColor;
	if (iAmMoving == false) {
		return;
	}

	if (catchDestination == true) {
		catchFinishCell(pos);
	} else {
		catchStartCell(pos);
	}
}


void HumanPlayer::catchFinishCell(const Position& pos) {

	qDebug() << ":HumanPlayer:" << "catch destination";
	myMove.pos2 = pos;
	myBoardView->clearHightlights();

	catchDestination = false;

	myMove.player = myColor;

	if (myModel->canMove(myMove) == false) {
		return;
	}

	iAmMoving = false;


	MOVES avMoves = myModel->movesFromPosition(myColor, myMove.pos1);
	bool accepted;
	MOVES::iterator itMove;
	for ( accepted = false, itMove = avMoves.begin() ; !accepted && itMove != avMoves.end(); ++itMove ) {
		if ( itMove->pos1 == myMove.pos1 && itMove->pos2 == myMove.pos2) {
			accepted = true;
			myMove = *itMove;
		}
	}

	notifyMoveReady(myMove);
}

void HumanPlayer::catchStartCell(const Position& pos) {

	qDebug() << ":HumanPlayer:" << "catch start cell";

	if (myModel->getBoardCell(pos.myX,pos.myY).player != myColor) {
		return;
	}

	MOVES moves = myModel->movesFromPosition(myColor, pos);

	qDebug() << ":HumanPlayer:" << "moves from position";
	if ( moves.empty() ) {
		return;
	}
	qDebug() << ":HumanPlayer:" << "moves empty";
	catchDestination = true;
	HIGHLIGHT_CELLS highlightCells;
	MOVES::iterator itMove;
	for ( itMove=moves.begin() ; itMove != moves.end(); ++itMove ) {
		QColor color = itMove->type & MOVE ? Qt::cyan : Qt::red;
		highlightCells.push_back ( std::make_pair( itMove->pos2, color ) );
	}

	myMove.pos1 = pos;

	myBoardView->highlight(highlightCells);
}

void HumanPlayer::makeTurn() {

	iAmMoving = true;
	catchDestination = false;
}

