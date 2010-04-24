/*
 * parallel.cpp
 *
 *  Created on: 02.04.2010
 *      Author: snowwlex
 */

#include <pthread.h>

#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "alphabeta_parallel_search_ai_player.h"

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void* parallelSearch(void* ptr) {
	Pull* pull = (Pull*)ptr;
	bool stop = false;
	Border alpha(0),beta(0);
	Move move;

	while (stop == false) {
		pthread_mutex_lock(&mutex1);
		if (pull->deque.empty() == false) {
			move = pull->deque.front();
			pull->deque.pop_front();
			alpha = pull->curAlpha;
			beta = pull->curBeta;
		} else {
			stop = true;
		}
		pthread_mutex_unlock(&mutex1);

		if (stop == false) {
			Model curModel = *pull->player->myModel;
			curModel.makeMove(move);
			int score = -pull->player->alphaBetaNegaMaxSearch(-beta, -alpha, 1 - pull->player->myColor, pull->player->myDepth-1 , curModel);

			qDebug() << ":AlphaBetaParallelSearchAIPlayer: "
					 << move.pos1.myX+'a' << pull->player->myModel->getBoardSizeY() - move.pos1.myY + '0'
					 << move.pos2.myX+'a' << pull->player->myModel->getBoardSizeY() - move.pos2.myY + '0'
					 << move.effect
					 << move.type
					 << move.player
					 << pull->player->myModel->getFigureData(move.figureId).letter
					 << score
					 << pthread_self();

			pthread_mutex_lock(&mutex1);

			if (pull->curAlpha < score) {
				pull->bestMove = move;
				pull->curAlpha = score;
			} else if (pull->curAlpha == score && rand()%2) {
				pull->bestMove = move;
			}

			pthread_mutex_unlock(&mutex1);
		}

	}

	qDebug() << ":AlphaBetaParallelSearchAIPlayer: " << "Thread terminate";

	pthread_mutex_lock(&mutex1);
	pull->threadsLeft -= 1;
	if (pull->threadsLeft == 0) {
		pull->player->allDone();
	}
	pthread_mutex_unlock(&mutex1);


}


AlphaBetaParallelSearchAIPlayer::AlphaBetaParallelSearchAIPlayer(int depth, int addDepth)
	: AlphaBetaSearchAIPlayer (depth, addDepth)
{

}

void AlphaBetaParallelSearchAIPlayer::makeTurn() {

	++myTurnsCounter;
	myCounter = 0;
	myQCounter = 0;
	srand ( time(0) );


	MOVES moves = myModel->allMoves(myColor, MOVE|CAPTURE);
	MOVES::iterator itMove;
	std::sort(moves.begin(),moves.end(), *this);
	myPull = Pull();
	myPull.deque.insert(myPull.deque.begin(), moves.begin(), moves.end());
	myPull.player = this;


	pthread_t thread1, thread2;
	myPull.threadsLeft = 2;
	pthread_create( &thread1, NULL, parallelSearch, (void*) &myPull);
	pthread_create( &thread2, NULL, parallelSearch, (void*) &myPull);
}

void AlphaBetaParallelSearchAIPlayer::allDone() {
	Move bestMove = myPull.bestMove;

	qDebug() << ":AlphaBetaSearchAIPlayer:"
			 << "My AB bestMove #" << myTurnsCounter
			 << "[" << bestMove.pos1.myX+'a' << myModel->getBoardSizeY() - bestMove.pos1.myY+'0'
			 << "-" << bestMove.pos2.myX+'a' << myModel->getBoardSizeY() - bestMove.pos2.myY+'0'
			 << "] (" << bestMove.type;
	qDebug() << ":AlphaBetaSearchAIPlayer:"
			 << "[AB] My counter= " << myCounter
			 << "qcounter="         << myQCounter
			 << "myDepth = "        << myDepth
			 << "turns ="           << myTurnsCounter;
	//qDebug() << ":AlphaBetaParallelSearchAIPlayer:" << "Time for searching:" << difftime(end,start);

	notifyMoveReady(bestMove);
}


