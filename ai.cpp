/*
 * ai.cpp
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#include <cstdlib>
#include <ctime>
#include <ncurses.h>
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
	srand ( myTurnsCounter*randomizator );
	debugView->clear();

	time(&start);
	score = alphaBetaNegaMaxSearch( move, Border(0,-INF), Border(0,INF), myColor, myDepth, *myModel);
	time(&end);

	sprintf(buffer, "My AB move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect ); myUserView->render(buffer);
	sprintf(buffer,"[AB] My counter = %d, myDepth = %d, turns = %d\n",myCounter, myDepth, myTurnsCounter); debugView->render(buffer);
	sprintf(buffer,"Time for AB searching: %f\n",difftime(end,start)); debugView->render(buffer);
	fflush(statfile);
	command = TURN;
	return command;
}


int AlphaBetaSearchAIPlayer::alphaBetaNegaMaxSearch(Move& returnMove, Border alpha, Border beta, int curPlayer, int curDepth , const Model& model) {
	MOVES moves;
	Move bestMove;
	MOVES::iterator itMove;
	int tmpScore;
	bool pruning;
	++myCounter;

	if (curDepth <= 0) {
		//fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"sef = %d\n",sef(&m,curPlayer)); fputs(buffer,statfile);
		return sefMaterial(model, curPlayer);
	}
	moves = model.allMoves(curPlayer);
	if (moves.empty()) {
		//fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"sef = %d\n",sef(&m,curPlayer)); fputs(buffer,statfile);
		return sefMaterial(model, curPlayer);
	}

	//fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"[%d] BEFORE: %d:%d\n",curDepth, alpha.myIsInfinity==0?alpha.myValue:999, beta.myIsInfinity==0?beta.myValue:999); fputs(buffer,statfile);
	Border score(0, -INF);

	for (pruning = false, itMove = moves.begin(); !pruning &&  itMove != moves.end(); ++itMove) {

		Model curModel = model;
		curModel.makeMove(*itMove);


		if ( alpha.myIsInfinity == 0) {
			// null window search
			Border newAlpha = alpha;
			newAlpha.myValue += 1;
			tmpScore = -alphaBetaNegaMaxSearch(bestMove, -newAlpha,-alpha,1-curPlayer, curDepth-1,curModel);
			if (alpha < tmpScore && beta > tmpScore) {
				tmpScore = -alphaBetaNegaMaxSearch(bestMove, -beta,-alpha,1-curPlayer, curDepth-1,curModel);
			}
			if (curDepth == myDepth) {
				sprintf(buffer,"[++] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,tmpScore); debugView->render(buffer);
				//debugView->wait();
			}
		} else {
			tmpScore = -alphaBetaNegaMaxSearch(bestMove, -beta,-alpha,1-curPlayer, curDepth-1,curModel);
			if (curDepth == myDepth) {
				sprintf(buffer,"[AB] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,tmpScore); debugView->render(buffer);
				//debugView->wait();
			}
		}

		//if (result == score && rand()%2) { returnMove = *itMove;}
		if (score  < tmpScore)  { score = tmpScore; returnMove = *itMove; }
		if (alpha  < score)  alpha = score;
		if (beta   < alpha ) pruning = true;
		//if (/*curDepth < myDepth &&*/  beta == alpha) pruning = true;

		//fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"[%d] AFTER: result=%d, %d:%d\n",curDepth, result.myIsInfinity==0?result.myValue:999,alpha.myIsInfinity==0?alpha.myValue:999, beta.myIsInfinity==0?beta.myValue:999);fputs(buffer,statfile);

	}
	//fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile);sprintf(buffer,"[%d] RETURN: %d\n",curDepth, result.myIsInfinity==0?result.myValue:999); fputs(buffer,statfile);


	if (score.myIsInfinity != 0)
		{ sprintf(buffer,"[%d] Exception! infinity return\n",curDepth); debugView->render(buffer); }

	return score.myValue;
}


