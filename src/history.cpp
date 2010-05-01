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

void History::clear() {
	myMoves.clear();
	myModels.clear();
}

const MOVES& History::getMoves() const {
	return myMoves;
}


