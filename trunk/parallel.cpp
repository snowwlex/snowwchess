/*
 * parallel.cpp
 *
 *  Created on: 02.04.2010
 *      Author: snowwlex
 */

#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <algorithm>
#include <vector>
#include <deque>
#include <string>
#include <map>

#include "snowwchess.h"
#include "model.h"
#include "view.h"
#include "player.h"
#include "ai.h"



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

			sprintf(buffer,"[Pab] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d thread is %ld]\n",move.pos1.myX+'a',pull->player->myModel->getBoardSizeY() - move.pos1.myY + '0',move.pos2.myX+'a',pull->player->myModel->getBoardSizeY() - move.pos2.myY + '0',move.effect,move.type,move.player,pull->player->myModel->getFigureData(move.figureId).letter,score, pthread_self()); debugView->render(buffer);

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

	DEBUG_DISPLAY("Thread terminate\n")

}


AlphaBetaParallelSearchAIPlayer::AlphaBetaParallelSearchAIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView, int depth):
		AIPlayer(color,model,boardView,userView,depth) { }

PlayerCommand AlphaBetaParallelSearchAIPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	int score;
	time_t start,end;


	switch(message) {
		case GOT_CHECK:
				myUserView->render(": I'm gotta check!\n");
				break;
		default:
				break;
	}

	++myTurnsCounter;
	myCounter = 0;
	myQCounter = 0;
	srand ( myTurnsCounter*randomizator );
	debugView->clear();


	sprintf(buffer,"Turn #%d, player %s\n",myTurnsCounter, myColor==0?"WHITE":"BLACK"); fputs(buffer,statfile);

	MOVES moves = myModel->allMoves(myColor, MOVE|CAPTURE);
	MOVES::iterator itMove;
	std::sort(moves.begin(),moves.end(), *this);
	myPull = Pull();
	myPull.deque.insert(myPull.deque.begin(), moves.begin(), moves.end());
	myPull.player = this;


//	for (PULL_QUEUE::iterator itMove = pull.deque.begin(); itMove != pull.deque.end(); ++itMove) {
//		DEBUG_DISPLAY_MOVE;
//	}
//	debugView->wait();
//	debugView->clear();

	pthread_t thread1, thread2;
	time(&start);
	pthread_create( &thread1, NULL, parallelSearch, (void*) &myPull);
	pthread_create( &thread2, NULL, parallelSearch, (void*) &myPull);

	pthread_join (thread1, NULL);
	pthread_join (thread2, NULL);

	time(&end);
	move = myPull.bestMove;

	sprintf(buffer, "My Pab move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect ); myUserView->render(buffer);
	sprintf(buffer,"[Pab] My counter = %d, qcounter=%d,myDepth = %d, turns = %d\n",myCounter, myQCounter, myDepth, myTurnsCounter); debugView->render(buffer);
	sprintf(buffer,"Time for PAB searching: %f\n",difftime(end,start)); debugView->render(buffer);
	fflush(statfile);
	command = TURN;
	return command;
}



int AlphaBetaParallelSearchAIPlayer::alphaBetaNegaMaxSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {

	curPlayer == myColor ? alpha = std::max(myPull.curAlpha,alpha) :  beta = std::min(-myPull.curAlpha,beta);

	MOVES moves;
	MOVES::iterator itMove;
	int tmpScore, tmp;
	bool pruning;
	++myCounter;


	if (curDepth <= 0) {
		DEBUG_LOG_SEF
		tmp = quiesSearch(alpha,beta,curPlayer, curDepth,model);
		//tmp = sefMaterial(model, curPlayer);
		return tmp;
	}


	moves = model.allMoves(curPlayer, MOVE|CAPTURE);
	if (moves.empty()) {
		DEBUG_LOG_SEF
		return sefMaterial(model, curPlayer);
	}
	DEBUG_LOG_BEFORE
	Border score(0, -INF);

	std::sort(moves.begin(),moves.end(), *this);

	for (pruning = false, itMove = moves.begin(); !pruning &&  itMove != moves.end(); ++itMove) {

		Model curModel = model;
		curModel.makeMove(*itMove);

		if (alpha.myIsInfinity == 0) { // NULL WINDOW search
			Border newAlpha = alpha;
			newAlpha.myValue += 1;
			tmpScore = -alphaBetaNegaMaxSearch(-newAlpha,-alpha,1-curPlayer, curDepth-1,curModel);
			if (alpha < tmpScore && beta > tmpScore) {
				tmpScore = -alphaBetaNegaMaxSearch(-beta,-alpha,1-curPlayer, curDepth-1,curModel);
			}
		} else {
			tmpScore = -alphaBetaNegaMaxSearch(-beta,-alpha,1-curPlayer, curDepth-1,curModel);
		}

		if (score  < tmpScore)  { score = tmpScore; }
		if (alpha  < score)  alpha = score;
		if (beta   < alpha ) pruning = true;



	}


	DEBUG_LOG_RETURN
	if (score.myIsInfinity != 0)
		{ sprintf(buffer,"[%d] Exception! infinity return\n",curDepth); debugView->render(buffer); }
	return score.myValue;
}


int AlphaBetaParallelSearchAIPlayer::quiesSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {

	curPlayer == myColor ? alpha = std::max(myPull.curAlpha,alpha) :  beta = std::min(-myPull.curAlpha,beta);

	MOVES moves;
	Move bestMove;
	MOVES::iterator itMove;
	int tmpScore;
	bool pruning;
	++myQCounter;


	if (model.isCheck(curPlayer) == true) {
		return alphaBetaNegaMaxSearch(alpha, beta, curPlayer, 1 , model);
	}

	Border score = sefMaterial(model,curPlayer);

	if (curDepth < -3) return score.myValue;
	moves = model.allMoves(curPlayer, CAPTURE);

	//sprintf(buffer,"[%d]",curDepth); debugView->render(buffer);
	//sprintf(buffer,"CAPTURE MOVES, deep=%d:\n",curDepth); debugView->render(buffer);
	//DEBUG_DISPLAY_BEFORE
	for (MOVES::const_iterator itMove = moves.begin(); itMove != moves.end(); ++itMove ) {
		//DEBUG_DISPLAY_MOVE
	}
	//if (curDepth == -4) debugView->wait();

	if (moves.empty()) {
		//DEBUG_DISPLAY("MOVES ARE OFF:\n");
		return score.myValue;
	}
	sort(moves.begin(),moves.end(),*this);
	if (alpha < score) alpha = score;

	for (pruning = false, itMove = moves.begin(); !pruning &&  itMove != moves.end(); ++itMove) {

		Model curModel = model;
		curModel.makeMove(*itMove);

		tmpScore = -quiesSearch(-beta,-alpha,1-curPlayer, curDepth-1,curModel);

		if (alpha  < tmpScore)  { alpha = tmpScore; }
		if (beta   < alpha ) pruning = true;
		if (beta == alpha) pruning = true;
	}

	if (alpha.myIsInfinity != 0)
		{ sprintf(buffer,"[%d] Exception! infinity return at quies search\n",curDepth); debugView->render(buffer); }
	//sprintf(buffer,"Returning alpha, deep=%d\n",curDepth); debugView->render(buffer);
	return alpha.myValue;

}

bool AlphaBetaParallelSearchAIPlayer::operator()(const Move& move1,const Move& move2) {
	//MVV - LVA
	if (move1.type == MOVE && move2.type == MOVE) {
		int movingFigure1 = myModel->getFigureIdOnBoard(move1.pos1.myX,move1.pos1.myY);
		int movingFigure2 = myModel->getFigureIdOnBoard(move2.pos1.myX,move2.pos1.myY);
		if (myModel->getFigureData(movingFigure1).weight > myModel->getFigureData(movingFigure2).weight) return true;
		return false;
	}
	if (move1.type == CAPTURE && move2.type == MOVE) return true;
	if (move1.type == MOVE && move2.type == CAPTURE) return false;
	if (move1.type == INPASSING || move2.type == INPASSING) return false;
	int movingFigure1 = myModel->getFigureIdOnBoard(move1.pos1.myX,move1.pos1.myY);
	int capturingFigure1 = myModel->getFigureIdOnBoard(move1.pos2.myX,move1.pos2.myY);
	int movingFigure2 = myModel->getFigureIdOnBoard(move2.pos1.myX,move2.pos1.myY);
	int capturingFigure2 = myModel->getFigureIdOnBoard(move2.pos2.myX,move2.pos2.myY);
	if (myModel->getFigureData(capturingFigure1).weight > myModel->getFigureData(capturingFigure2).weight) return true;
	if (myModel->getFigureData(capturingFigure1).weight < myModel->getFigureData(capturingFigure2).weight) return false;
	if (myModel->getFigureData(movingFigure1).weight < myModel->getFigureData(movingFigure2).weight) return true;
	if (myModel->getFigureData(movingFigure1).weight > myModel->getFigureData(movingFigure2).weight) return false;
	return false;
}
