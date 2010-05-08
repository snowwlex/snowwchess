/*
 * alphabeta_parallel_search_ai_player.h
 *
 *  Created on: 24.04.2010
 *      Author: snowwlex
 */

#ifndef ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_
#define ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_

#include <QThread>
#include <QMutex>

#include <deque>

#include "../snowwchess.h"
#include "alphabeta_search_ai_player.h"

typedef std::deque<Move> PULL_QUEUE;

struct Pull {
	QMutex mutex;
	PULL_QUEUE deque;
	Border curAlpha;
	Border curBeta;
	Move bestMove;
	Pull(): curAlpha(0,-INF), curBeta(0,INF) { }
};




class AlphaBetaParallelSearchAIPlayer : public AlphaBetaSearchAIPlayer {

	public:
		AlphaBetaParallelSearchAIPlayer(int depth=0, int addDepth=0);

	public:
		virtual void makeTurn();

	private:
		void allDone();

	private:
		Pull* myPull;

	friend class ParallelSearchProcess;
	friend class ParallelSearch;


};

class ParallelSearch : public QThread {

	public:
		ParallelSearch(AlphaBetaParallelSearchAIPlayer* player, Pull* pull);
	public:
		virtual void run();

	private:
		AlphaBetaParallelSearchAIPlayer* myPlayer;
		Pull* myPull;

};

class ParallelSearchProcess : public QThread {

	public:
		ParallelSearchProcess(AlphaBetaParallelSearchAIPlayer* player, Pull* pull);
	public:
		virtual void run();

	private:
		AlphaBetaParallelSearchAIPlayer* myPlayer;
		Pull* myPull;

};


#endif /* ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_ */
