/*
 * alphabeta_parallel_search_ai_player.h
 *
 *  Created on: 24.04.2010
 *      Author: snowwlex
 */

#ifndef ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_
#define ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_

#include <deque>

#include "../snowwchess.h"
#include "alphabeta_search_ai_player.h"

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

#endif /* ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_ */
