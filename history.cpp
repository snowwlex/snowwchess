/*
 * history.cpp
 *
 *  Created on: 02.04.2010
 *      Author: snowwlex
 */
#include <vector>
#include <string>
#include <map>

#include "snowwchess.h"
#include "model.h"
#include "history.h"

const Model& History::makeUndo(int quantityMoves) {
	for (int i = 0; i < quantityMoves; ++i) {
		myMoves.pop_back();
		myModels.pop_back();
	}
	return myModels.back();
}



void History::addRecord(const Move & move, const Model & model) {
	myMoves.push_back(move);
	myModels.push_back(model);
}



const MOVES& History::getMoves() const {
	return myMoves;
}


