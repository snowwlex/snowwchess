/*
 * parallel.cpp
 *
 *  Created on: 02.04.2010
 *      Author: snowwlex
 */

#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "alphabeta_parallel_search_ai_player.h"

#include "../view/gui_history_view.h"

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
	myPull = new Pull;
	myPull->deque.insert(myPull->deque.begin(), moves.begin(), moves.end());


	ParallelSearch process(this, myPull);

	process.start();


}

void AlphaBetaParallelSearchAIPlayer::allDone() {
	Move bestMove = myPull->bestMove;

	qDebug() << ":AlphaBetaSearchAIPlayer:"
			 << "My AB bestMove #" << myTurnsCounter
			 << GuiHistoryView::moveToString(bestMove, *myModel);
	qDebug() << ":AlphaBetaSearchAIPlayer:"
			 << "[AB] My counter= " << myCounter
			 << "qcounter="         << myQCounter
			 << "myDepth = "        << myDepth
			 << "turns ="           << myTurnsCounter;

	qWarning() << ":AlphaBetaParallelSearchAIPlayer: "
			   << "counter: " << myCounter;

	delete myPull;
	notifyMoveReady(bestMove);
}



ParallelSearch::ParallelSearch(AlphaBetaParallelSearchAIPlayer* player, Pull* pull) {
	myPlayer = player;
	myPull = pull;
}

void ParallelSearch::run() {

	ParallelSearchProcess process1(myPlayer, myPull);
	ParallelSearchProcess process2(myPlayer, myPull);
	qDebug() << ":ParallelSearch:" << "process started";
	process1.start();
	process2.start();

	qDebug() << ":ParallelSearch:" << "waiting for process";
	process1.wait();
	process2.wait();

	qDebug() << ":ParallelSearch:" << "process ended";

	//myPlayer->allDone();


}

void ParallelSearchProcess::run() {

	bool stop = false;
	Border alpha(0),beta(0);
	Move move;

	while (stop == false) {

		myPull->mutex.lock();
		if (myPull->deque.empty() == false) {
			move = myPull->deque.front();
			myPull->deque.pop_front();
			alpha = myPull->curAlpha;
			beta = myPull->curBeta;
		} else {
			stop = true;
		}
		myPull->mutex.unlock();

		if (stop == false) {
			Model curModel = *myPlayer->myModel;
			curModel.makeMove(move);
			int score = -myPlayer->alphaBetaNegaMaxSearch(-beta, -alpha, 1 - myPlayer->myColor, myPlayer->myDepth-1 , curModel);

			qDebug() << ":AlphaBetaParallelSearchAIPlayer: "
					 << GuiHistoryView::moveToString(move, *myPlayer->myModel)
					 << score
					 << this->currentThreadId();

			myPull->mutex.lock();

			if (myPull->curAlpha < score) {
				myPull->bestMove = move;
				myPull->curAlpha = score;
			} else if (myPull->curAlpha == score && rand()%2) {
				myPull->bestMove = move;
			}

			myPull->mutex.unlock();

			qWarning() << ":AlphaBetaParallelSearchAIPlayer: "
					   << "move counted, score: " << score
					   << "thread: " << this->currentThreadId();

		}

	}

	qDebug() << ":AlphaBetaParallelSearchAIPlayer: " << "Thread terminate";

}

ParallelSearchProcess::ParallelSearchProcess(AlphaBetaParallelSearchAIPlayer* player, Pull* pull) {
	myPlayer = player;
	myPull = pull;
}

