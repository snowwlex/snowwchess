/*
 * history.h
 *
 *  Created on: 02.04.2010
 *      Author: snowwlex
 */

#ifndef HISTORY_H_
#define HISTORY_H_

class History {
	private:
		MOVES myMoves;
		std::vector< Model > myModels;
	public:
		void addRecord(const Move& move, const Model& model);
		const MOVES& getMoves() const;
		const Model& makeUndo(int quantityMoves=1);

};
#endif /* HISTORY_H_ */
