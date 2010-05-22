/*
 * ai.cpp
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#include "ai_player.h"

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




bool sortMVV_LVA::operator()(const Move& move1,const Move& move2) {
	//sort by principe: Most Value Victim - Least Value Actor (MVV - LVA)
	if (move1.type == MOVE && move2.type == MOVE) {
		if (myModel->getFigureData(move1.figureId).weight > myModel->getFigureData(move2.figureId).weight) return true;
		return false;
	}
	if (move1.type == CAPTURE && move2.type == MOVE) return true;
	if (move1.type == MOVE && move2.type == CAPTURE) return false;
	if (move1.type == INPASSING || move2.type == INPASSING) return false;
        
        assert(move1.figureCapturedId != 0);
        assert(move2.figureCapturedId != 0);
	if (myModel->getFigureData(move1.figureCapturedId).weight > myModel->getFigureData(move2.figureCapturedId).weight) return true;
	if (myModel->getFigureData(move1.figureCapturedId).weight < myModel->getFigureData(move2.figureCapturedId).weight) return false;
	if (myModel->getFigureData(move1.figureId).weight < myModel->getFigureData(move2.figureId).weight) return true;
	if (myModel->getFigureData(move1.figureId).weight > myModel->getFigureData(move2.figureId).weight) return false;
	return false;
}

void AIPlayer::setDepth(int depth) {
	myDepth = depth;
}

void AIPlayer::setAddDepth(int depth) {
	myAddDepth = depth;
}



