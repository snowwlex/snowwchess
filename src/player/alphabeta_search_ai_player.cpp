/*
 * ai_ab_search.cpp
 *
 *  Created on: 24.04.2010
 *      Author: snowwlex
 */
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "alphabeta_search_ai_player.h"

AlphaBetaSearchAIPlayer::AlphaBetaSearchAIPlayer(int depth, int addDepth) {
	myDepth = depth;
	myAddDepth = addDepth;
}


void AlphaBetaSearchAIPlayer::makeTurn() {

	time_t start,end;

	++myTurnsCounter;
	myCounter = 0;
	myQCounter = 0;
	srand ( time(0) );


	MOVES moves = myModel->allMoves(myColor, MOVE|CAPTURE);
	MOVES::iterator itMove;
	Move bestMove;
	Border alpha(0,-INF),  beta(0,INF) ;

	time(&start);
	for (itMove = moves.begin(); itMove != moves.end(); ++itMove) {
		++myCounter;
		Model curModel = *myModel;
		curModel.makeMove(*itMove);
		int score = -alphaBetaNegaMaxSearch(-beta, -alpha, 1 - myColor, myDepth-1 , curModel);
		if (alpha < score) {
			alpha = score;
			bestMove = *itMove;
		} else if (alpha == score && rand()%2) {
			bestMove = *itMove;
		}
	}
	time(&end);

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
	qDebug() << ":AlphaBetaSearchAIPlayer:" << "Time for AB searching:" << difftime(end,start);

	notifyMoveReady(bestMove);
}


int AlphaBetaSearchAIPlayer::alphaBetaNegaMaxSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {


	MOVES moves;
	MOVES::iterator itMove;
	int tmpScore, tmp;
	bool pruning;
	++myCounter;


	if (curDepth <= 0) {
		tmp = quiesSearch(alpha,beta,curPlayer, curDepth,model);
		//tmp = sefMaterial(model, curPlayer);
		return tmp;
	}

	moves = model.allMoves(curPlayer, MOVE|CAPTURE);
	if (moves.empty()) {
		return sefMaterial(model, curPlayer);
	}
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
	return score.myValue;
}


int AlphaBetaSearchAIPlayer::quiesSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {

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

	if (curDepth <= myAddDepth) return score.myValue;
	moves = model.allMoves(curPlayer, CAPTURE);

	if (moves.empty()) {
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

	return alpha.myValue;
}
