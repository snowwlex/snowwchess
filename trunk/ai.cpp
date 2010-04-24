/*
 * ai.cpp
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

//#include <ncurses.h>


#include "snowwchess.h"
#include "ai.h"

int AIPlayer::sefMaterial(const Model& model, int player) const {
	FIGURES::const_iterator itFigure;
	int myScore, oppScore;
	for (myScore = 0, itFigure = model.getSetFigures(player).begin(); itFigure != model.getSetFigures(player).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			myScore += model.getFigureData(itFigure->id).weight;
		}
	}
	for (oppScore = 0, itFigure = model.getSetFigures(1-player).begin(); itFigure != model.getSetFigures(1-player).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			oppScore += model.getFigureData(itFigure->id).weight;
		}
	}
	return myScore-oppScore;

}

bool AIPlayer::operator()(const Move& move1,const Move& move2) {
	//sort by principe: Most Value Victim - Least Value Actor (MVV - LVA)
	if (move1.type == MOVE && move2.type == MOVE) {
		int movingFigure1 = myModel->getFigureIdOnBoard(move1.pos1.myX,move1.pos1.myY);
		int movingFigure2 = myModel->getFigureIdOnBoard(move2.pos1.myX,move2.pos1.myY);
		if (myModel->getFigureData(movingFigure1).weight > myModel->getFigureData(movingFigure2).weight) return true;
		return false;
	}
	if (move1.type == CAPTURE && move2.type == MOVE) return true;
	if (move1.type == MOVE && move2.type == CAPTURE) return false;
	if (move1.type == INPASSING || move2.type == INPASSING) return false;
	int movingFigure1 = myModel->getFigureIdOnBoard(move1.pos1.myX,move1.pos1.myY);
	int capturingFigure1 = myModel->getFigureIdOnBoard(move1.pos2.myX,move1.pos2.myY);
	int movingFigure2 = myModel->getFigureIdOnBoard(move2.pos1.myX,move2.pos1.myY);
	int capturingFigure2 = myModel->getFigureIdOnBoard(move2.pos2.myX,move2.pos2.myY);
	if (myModel->getFigureData(capturingFigure1).weight > myModel->getFigureData(capturingFigure2).weight) return true;
	if (myModel->getFigureData(capturingFigure1).weight < myModel->getFigureData(capturingFigure2).weight) return false;
	if (myModel->getFigureData(movingFigure1).weight < myModel->getFigureData(movingFigure2).weight) return true;
	if (myModel->getFigureData(movingFigure1).weight > myModel->getFigureData(movingFigure2).weight) return false;
	return false;
}

void AIPlayer::setDepth(int depth) {
	myDepth = depth;
}

void AIPlayer::setAddDepth(int depth) {
	myAddDepth = depth;
}



