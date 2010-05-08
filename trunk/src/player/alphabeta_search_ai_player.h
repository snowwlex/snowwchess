/*
 * alphabeta_search_ai_player.h
 *
 *  Created on: 24.04.2010
 *      Author: snowwlex
 */

#ifndef ALPHABETA_SEARCH_AI_PLAYER_H_
#define ALPHABETA_SEARCH_AI_PLAYER_H_

#include "ai_player.h"

enum Infinity { INF=1 };
struct Border { // for alpha-beta borders
	int myIsInfinity;
	int myValue;
	Border(int value, int isInfinity=0) : myIsInfinity(isInfinity), myValue(value) { }
	Border operator-() const {
		return Border(-myValue, -myIsInfinity);
	}
	bool operator<(const Border& border) const {
		if (myIsInfinity > border.myIsInfinity) return false;
		if (myIsInfinity < border.myIsInfinity) return true;
		if (myIsInfinity == 0 && myValue < border.myValue) return true;
		return false;
	}
	bool operator>(const Border& border) const {
		if (myIsInfinity > border.myIsInfinity) return true;
		if (myIsInfinity < border.myIsInfinity) return false;
		if (myIsInfinity == 0 && myValue > border.myValue) return true;
		return false;
	}
	bool operator==(const Border& border) const {
		return myIsInfinity==border.myIsInfinity && myValue==border.myValue;
	}
};

class AlphaBetaSearchAIPlayer : public AIPlayer {
	public:
		AlphaBetaSearchAIPlayer(int depth=1, int addDepth=0);

	public:
		virtual void getMove();

	protected:
		enum Infinity { INF=1 };

	protected: //searching methods

		int alphaBetaNegaMaxSearch(Border alpha, Border beta, int curPlayer, int curDepth, const Model& model);
		int quiesSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model);

};

#endif /* ALPHABETA_SEARCH_AI_PLAYER_H_ */
