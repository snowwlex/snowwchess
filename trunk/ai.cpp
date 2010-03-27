/*
 * ai.cpp
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

#include "snowwchess.h"
#include "model.h"
#include "view.h"
#include "player.h"
#include "ai.h"

AIPlayer::AIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView, int depth): myColor(color), myModel(model),
					myBoardView(boardView), myUserView(userView) {
	//time(0) );
	myDepth = depth;
	myTurnsCounter = 0;
}

int AIPlayer::sefMaterial(const Model& model, int player) const {
	FIGURES::const_iterator itFigure;
	int myScore, oppScore;
	for (myScore = 0, itFigure = model.getSetFigures(player).begin(); itFigure != model.getSetFigures(player).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			myScore += model.getFigureData(itFigure->id).weight;
		}
	}
	for (oppScore = 0, itFigure = model.getSetFigures(1-player).begin(); itFigure != model.getSetFigures(1-player).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			oppScore += model.getFigureData(itFigure->id).weight;
		}
	}
	return myScore-oppScore;

}


FullSearchAIPlayer::FullSearchAIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView, int depth):
		AIPlayer(color,model,boardView,userView,depth) { }

PlayerCommand FullSearchAIPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	int score;
	time_t start,end;

	switch(message) {
		case GOT_CHECK:
				myUserView->render("I'm gotta check!\n");
				break;
		default:
				break;
	}


	++myTurnsCounter;
	myCounter = 0;
	srand ( myTurnsCounter*randomizator );
	debugView->clear();

	time(&start);
	score = miniMaxSearch( move, myDepth, myColor, *myModel);
	time(&end);
	sprintf(buffer, "[US] My move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect ); myUserView->render(buffer); //myUserView->wait();
	sprintf(buffer,"My counter = %d, myDepth = %d, turns = %d\n",myCounter, myDepth, myTurnsCounter); debugView->render(buffer);
	sprintf(buffer,"Time for searching: %f\n",difftime(end,start)); debugView->render(buffer);
	command = TURN;
	return command;
}


int FullSearchAIPlayer::miniMaxSearch(Move& returnMove, int curDepth, int curPlayer, const Model& model) {
	MOVES moves;
	Move bestMove;
	MOVES::iterator itMove;
	int score, tmpScore;
	bool scored;

	++myCounter;

	if (curDepth == 0) {
		return sefMaterial(model,myColor);
	}

	moves = model.allMoves(curPlayer);

	if (moves.empty()) {
		return sefMaterial(model, myColor);
	}

	for (scored = false, score=0, itMove = moves.begin(); itMove != moves.end(); ++itMove) {
		Model curModel = model;
		curModel.makeMove(*itMove);
		tmpScore = miniMaxSearch(bestMove,curDepth-1, 1-curPlayer, curModel);
		if (curDepth == myDepth) {
			sprintf(buffer,"[US] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,tmpScore); debugView->render(buffer);
			//debugView->wait();
		}

		if (scored == false) {
			score = tmpScore;
			returnMove = *itMove;
			scored = true;
		} else {
			if ( (curPlayer == myColor && tmpScore > score) || ( curPlayer != myColor && tmpScore < score) ) {
				score = tmpScore;
				returnMove = *itMove;
			} else if (false && tmpScore == score && rand()%2 ) {
				returnMove = *itMove;
			}
		}
	}
	return score;
}


AlphaBetaSearchAIPlayer::AlphaBetaSearchAIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView, int depth):
		AIPlayer(color,model,boardView,userView,depth) { }

PlayerCommand AlphaBetaSearchAIPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;
	int score;
	time_t start,end;


	switch(message) {
		case GOT_CHECK:
				myUserView->render("AB: I'm gotta check!\n");
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
	time(&start);
	score = alphaBetaNegaMaxSearch( move, Border(0,-INF), Border(0,INF), myColor, myDepth, *myModel);
	time(&end);

	sprintf(buffer, "My AB move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect ); myUserView->render(buffer);
	sprintf(buffer,"[AB] My counter = %d, qcounter=%d,myDepth = %d, turns = %d\n",myCounter, myQCounter, myDepth, myTurnsCounter); debugView->render(buffer);
	sprintf(buffer,"Time for AB searching: %f\n",difftime(end,start)); debugView->render(buffer);
	fflush(statfile);
	command = TURN;
	return command;
}


int AlphaBetaSearchAIPlayer::quiesSearch(Move& returnMove, Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {

	MOVES moves;
	Move bestMove;
	MOVES::iterator itMove;
	int tmpScore;
	bool pruning;
	++myQCounter;


	if (model.isCheck(curPlayer) == true) {
		return alphaBetaNegaMaxSearch(bestMove, alpha, beta, curPlayer, 1 , model);
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

		tmpScore = -quiesSearch(bestMove, -beta,-alpha,1-curPlayer, curDepth-1,curModel);

		if (alpha  < tmpScore)  { alpha = tmpScore; returnMove = *itMove; }
		if (beta   < alpha ) pruning = true;
		if (beta == alpha) pruning = true;


	}

	if (alpha.myIsInfinity != 0)
		{ sprintf(buffer,"[%d] Exception! infinity return at quies search\n",curDepth); debugView->render(buffer); }
	//sprintf(buffer,"Returning alpha, deep=%d\n",curDepth); debugView->render(buffer);
	return alpha.myValue;

}


int AlphaBetaSearchAIPlayer::alphaBetaNegaMaxSearch(Move& returnMove, Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {
	MOVES moves;
	Move bestMove;
	MOVES::iterator itMove;
	int tmpScore, tmp;
	bool pruning;
	++myCounter;

	if (curDepth <= 0) {
		DEBUG_LOG_SEF
		//tmp = sefMaterial(model, curPlayer);
		//return tmp;
		tmpScore = quiesSearch(bestMove, alpha,beta,curPlayer, curDepth,model);
		return tmpScore;

	}

	//null move
	/*
	if (model.isCheck(curPlayer) == false && beta.myIsInfinity == 0 && curDepth < myDepth-1) {
		Border newBeta = beta;
		newBeta.myValue -= 1;
		tmpScore = -alphaBetaNegaMaxSearch(bestMove, -beta,-newBeta,1-curPlayer, curDepth-1-2,model);
		if ( beta < tmpScore ) {
			return tmpScore;
		}
	}
	*/



	moves = model.allMoves(curPlayer);
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

		if ( false && alpha.myIsInfinity == 0) {
			// null window search
			Border newAlpha = alpha;
			newAlpha.myValue += 1;
			tmpScore = -alphaBetaNegaMaxSearch(bestMove, -newAlpha,-alpha,1-curPlayer, curDepth-1,curModel);
			if (alpha < tmpScore && beta > tmpScore) {
				//if (curDepth==1) { sprintf(buffer,"        Searching at %d\n",myCounter); debugView->render(buffer);  }
				bestMove = *itMove;
				tmpScore = -alphaBetaNegaMaxSearch(bestMove, -beta,-alpha,1-curPlayer, curDepth-1,curModel);
			}
			if (curDepth == myDepth) {
				DEBUG_DISPLAY_SCORE
				DEBUG_LOG_SCORE
				//debugView->wait();
			}
		} else {

			//if (curDepth==1) { sprintf(buffer,"        Searching at %d\n",myCounter); debugView->render(buffer);  }
			bestMove = *itMove;
			tmpScore = -alphaBetaNegaMaxSearch(bestMove, -beta,-alpha,1-curPlayer, curDepth-1,curModel);
			if (curDepth == myDepth) {
				DEBUG_DISPLAY_SCORE
				DEBUG_LOG_SCORE
				//debugView->wait();
			}
		}

		if (score == tmpScore && rand()%2) { returnMove = *itMove;}
		if (score  < tmpScore)  { score = tmpScore; returnMove = *itMove; }
		if (alpha  < score)  alpha = score;
		if (beta   < alpha ) pruning = true;
		//if (beta == alpha) pruning = true;



	}


	DEBUG_LOG_RETURN


	if (score.myIsInfinity != 0)
		{ sprintf(buffer,"[%d] Exception! infinity return\n",curDepth); debugView->render(buffer); }

	return score.myValue;
}

bool AlphaBetaSearchAIPlayer::operator()(const Move& move1,const Move& move2) {
	//MVV - LVA
	if (move1.type == MOVE && move2.type == MOVE) {
		int movingFigure1 = myModel->getBoardCell(move1.pos1.myX,move1.pos1.myY);
		int movingFigure2 = myModel->getBoardCell(move2.pos1.myX,move2.pos1.myY);
		if (myModel->getFigureData(movingFigure1).weight > myModel->getFigureData(movingFigure2).weight) return true;
		return false;
	}
	if (move1.type == CAPTURE && move2.type == MOVE) return true;
	if (move1.type == MOVE && move2.type == CAPTURE) return false;

	if (move1.type == INPASSING || move2.type == INPASSING) return false;

	int movingFigure1 = myModel->getBoardCell(move1.pos1.myX,move1.pos1.myY);
	int capturingFigure1 = myModel->getBoardCell(move1.pos2.myX,move1.pos2.myY);
	int movingFigure2 = myModel->getBoardCell(move2.pos1.myX,move2.pos1.myY);
	int capturingFigure2 = myModel->getBoardCell(move2.pos2.myX,move2.pos2.myY);

	if (myModel->getFigureData(capturingFigure1).weight > myModel->getFigureData(capturingFigure2).weight) return true;
	if (myModel->getFigureData(capturingFigure1).weight < myModel->getFigureData(capturingFigure2).weight) return false;

	if (myModel->getFigureData(movingFigure1).weight < myModel->getFigureData(movingFigure2).weight) return true;
	if (myModel->getFigureData(movingFigure1).weight > myModel->getFigureData(movingFigure2).weight) return false;

	return false;


}

