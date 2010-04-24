/*
 * ai.h
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#ifndef AI_H_
#define AI_H_

#include <deque>

#include "snowwchess.h"
#include "player.h"

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

class AIPlayer : public Player {
	public:
		virtual void makeTurn() = 0;

	public: //setters
		void setDepth(int depth);
		void setAddDepth(int addDepth);

	public: //help methods
		bool operator()(const Move& move1,const Move& move2); //for moves sort

	protected:
		AIPlayer() { myTurnsCounter = 0; }

	protected:
		int sefMaterial(const Model& model, int player) const;

	protected:
		int myDepth;
		int myAddDepth;

	protected: //for debug
		int myTurnsCounter;
		int myCounter;
		int myQCounter;


};
//
//class FullSearchAIPlayer : public AIPlayer {
//	private:
//		int miniMaxSearch(Move& returnMove, int curDepth, int curPlayer, const Model& model);
//	public:
//		FullSearchAIPlayer();
//		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
//};
//

class AlphaBetaSearchAIPlayer : public AIPlayer {
	public:
		AlphaBetaSearchAIPlayer(int depth=1, int addDepth=0);

	public:
		virtual void makeTurn();

	protected:
		enum Infinity { INF=1 };

	protected: //searching methods

		int alphaBetaNegaMaxSearch(Border alpha, Border beta, int curPlayer, int curDepth, const Model& model);
		int quiesSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model);

};

class AlphaBetaParallelSearchAIPlayer;
typedef std::deque<Move> PULL_QUEUE;
struct Pull {
	AlphaBetaParallelSearchAIPlayer* player;
	PULL_QUEUE deque;
	Border curAlpha;
	Border curBeta;
	Move bestMove;
	int threadsLeft;
	Pull() :  curAlpha(0,-INF), curBeta(0,INF) { }
};

class AlphaBetaParallelSearchAIPlayer : public AlphaBetaSearchAIPlayer {

	public:
		AlphaBetaParallelSearchAIPlayer(int depth=0, int addDepth=0);

	public:
		virtual void makeTurn();

	private:
		friend void* parallelSearch(void*);
		void allDone();

	private:
		Pull myPull;

};

#endif /* AI_H_ */
