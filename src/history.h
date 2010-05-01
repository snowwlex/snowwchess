#ifndef HISTORY_H_
#define HISTORY_H_

#include <vector>

#include "snowwchess.h"
#include "model/model.h"

class History {
	public:
		void addRecord(const Move& move, const Model& model);
		const MOVES& getMoves() const;
		const Model& makeUndo(int quantityMoves=1);
		void clear();
	private:
		MOVES myMoves;
		std::vector< Model > myModels;

};
#endif /* HISTORY_H_ */
