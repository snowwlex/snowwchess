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

PlayerCommand AIPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;




	switch(message) {
		case GOT_CHECK:
				myUserView->render("I'm gotta check!\n");
				break;
		default:
				break;
	}

	++myTurnsCounter;
	//if (myDepth < 15) myDepth = (int)(myTurnsCounter / 4) ;
	//if (myDepth == 0) myDepth = 1;

	move = search();



	sprintf(buffer, "[US] My move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect );
	myUserView->render(buffer);
	myUserView->wait();
	command = TURN;
	return command;
}

Move AIPlayer::search() {
	Move move;
	MOVES moves;
	MOVES::iterator itMove;

	int maxScore=0, score,i,max;
	bool scored;

	myCounter = 0;
	debugView->clear();

	srand ( myTurnsCounter*randomizator );

	moves = myModel->moves(myColor);
	for (scored = false, max = i = 0, itMove = moves.begin(); itMove != moves.end(); ++itMove, ++i) {
		++myCounter;
		score = searchRecurs(*myModel, *itMove, 1, 0);
		//if (itMove->type == CAPTURE) score += 50;
		sprintf(buffer,"[US] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
		debugView->render(buffer);
		if (scored == false) {
			maxScore = score;
			scored = true;
			max = i;
		} else if (score > maxScore){
			maxScore = score;
			max = i;
		}
		else if (score == maxScore && rand()%2 == 1) {
			max = i;
		}
	}

	myBoardView->myModel = myModel;
	sprintf(buffer,"My counter = %d, myDepth = %d, turns = %d\n",myCounter, myDepth, myTurnsCounter); debugView->render(buffer);

	return moves[max];
}

int AIPlayer::searchRecurs(Model m, Move move, int curDepth, int max ) {
	MOVES moves;
	MOVES::iterator itMove;
	int curPlayerId;
	int optScore, score;
	bool scored;

	Move bestMove;


	m.makeMove(move);

	if (curDepth >= myDepth) {
		return sef(&m);
	}

	curPlayerId = max == 1 ? myColor : 1-myColor;
	moves = m.moves(curPlayerId);

	if (moves.size() == 0) {
		return sef(&m);
	}

	for (scored = false, optScore=0, itMove = moves.begin(); itMove != moves.end(); ++itMove) {
		++myCounter;
		score = searchRecurs(m, *itMove,curDepth+1, 1-max);

		if (scored == false) {
			optScore = score;
			scored = true;
			bestMove = *itMove;
			//if (itMove->type == CAPTURE) optScore += 30;
			//sprintf(buffer,"%s[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",std::string(curDepth*2,' ').c_str(),itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
			//debugView->render(buffer);
		} else if ( (max == 1 && score > optScore) || (max == 0 && score < optScore) ) {
				optScore = score;
				//if (itMove->type == CAPTURE && max == 1) optScore += (int)(100/curDepth);
				//if (itMove->type == CAPTURE && max == 0) optScore -= (int)(100/curDepth);
				bestMove = *itMove;
				//sprintf(buffer,"%s[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",std::string(curDepth*2,' ').c_str(),itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
				//debugView->render(buffer);
		}
	}


	//sprintf(buffer,"%s[BM] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d\n",std::string(curDepth*2,' ').c_str(),bestMove.pos1.myX+'a',myModel->getBoardSizeY() - bestMove.pos1.myY + '0',bestMove.pos2.myX+'a',myModel->getBoardSizeY() - bestMove.pos2.myY + '0',bestMove.effect,bestMove.type,bestMove.player,myModel->getFigureData(bestMove.figureId).letter,optScore);
	//debugView->render(buffer);

	if (myTurnsCounter > 500) {
		m.makeMove(bestMove);
		myBoardView->myModel = &m;
		myBoardView->render();
		myBoardView->highlight(bestMove.pos1,13);
		myBoardView->highlight(bestMove.pos2,11);
		//myBoardView->wait();
	}

	return optScore;
}

int AIPlayer::sef(Model *m) const {
	FIGURES::const_iterator itFigure;
	int myScore, oppScore;
	for (myScore = 0, itFigure = m->getSetFigures(myColor).begin(); itFigure != m->getSetFigures(myColor).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			myScore += m->getFigureData(itFigure->id).weight;
		}
	}
	for (oppScore = 0, itFigure = m->getSetFigures(1-myColor).begin(); itFigure != m->getSetFigures(1-myColor).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			oppScore += m->getFigureData(itFigure->id).weight;
		}
	}
	return myScore-oppScore;

}

/**********************************************************/


AlphaBetaAIPlayer::AlphaBetaAIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView, int depth): myColor(color), myModel(model),
					myBoardView(boardView), myUserView(userView) {
	//time(0) );
	myDepth = depth;
	myTurnsCounter = 0;
}

PlayerCommand AlphaBetaAIPlayer::makeTurn(Move& move, GameMessage message) {

	PlayerCommand command;




	switch(message) {
		case GOT_CHECK:
				myUserView->render("AB: I'm gotta check!\n");
				break;
		default:
				break;
	}

	++myTurnsCounter;
	//if (myDepth < 15) myDepth = (int)(myTurnsCounter / 4) ;
	//if (myDepth == 0) myDepth = 1;

	move = search();



	sprintf(buffer, "My AB move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect );
	myUserView->render(buffer);
	//myUserView->wait();
	command = TURN;
	return command;
}

Move AlphaBetaAIPlayer::search() {
	Move move;
	MOVES moves;
	MOVES::iterator itMove;
	AlphaBetaBorder borders;
	int maxScore=0, score,i,max;
	bool scored;

	myCounter = 0;
	debugView->clear();

	srand ( myTurnsCounter*randomizator );
 int depth=3;
 if (myTurnsCounter > 10 ) {
	 depth=4;
 }
 if (myTurnsCounter > 22 ) {
	 depth=5;
 }
 sprintf(buffer,"Thinking at deep %d:\n",depth); debugView->render(buffer);
	moves = myModel->moves(myColor);
	for (scored = false, max = i = 0, itMove = moves.begin(); itMove != moves.end(); ++itMove, ++i) {
		++myCounter;
		score = searchRecurs(*myModel, *itMove, depth, 0, borders);
		alphaBetaPruning(1, borders, score);
		sprintf(buffer,"[AB] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d] ",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
		debugView->render(buffer);
		if (borders.isLower) {
			sprintf(buffer,"[%d ",borders.lower);
			debugView->render(buffer);
		} else {
			sprintf(buffer,"[-INF ");
			debugView->render(buffer);
		}
		if (borders.isUpper) {
			sprintf(buffer,"%d]\n",borders.upper);
			debugView->render(buffer);
		} else {
			sprintf(buffer,"+INF]\n");
			debugView->render(buffer);
		}
		if (scored == false) {
			maxScore = score;
			scored = true;
			max = i;
		} else if (score > maxScore){
			maxScore = score;
			max = i;
		}
		else if (score == maxScore && rand()%2 == 1) {
			max = i;
		}
	}

	myBoardView->myModel = myModel;
	sprintf(buffer,"[AB] My counter = %d, myDepth = %d, turns = %d\n",myCounter, depth, myTurnsCounter); debugView->render(buffer);
	return moves[max];
}

bool AlphaBetaAIPlayer::alphaBetaPruning(int max, AlphaBetaBorder& borders, int score)
{
	bool pruning=false;
    if(max == 1){
        if(borders.isUpper && borders.upper < score){
            pruning = true;
        }
        if( (borders.isLower && borders.lower < score) || !borders.isLower){
            borders.isLower = true;
            borders.lower = score;
        }
    }
    else{
        if(borders.isLower && borders.lower > score){
            pruning = true;
        }
        if( (borders.isUpper && borders.upper > score) || !borders.isUpper){
            borders.isUpper = true;
            borders.upper = score;
        }
    }
    return pruning;
}

int AlphaBetaAIPlayer::searchRecurs(Model m, Move move, int curDepth, int max, AlphaBetaBorder borders ) {
	MOVES moves;
	MOVES::iterator itMove;
	bool pruning;
	int curPlayerId;
	int optScore, score;
	bool scored;

	Move bestMove;


	m.makeMove(move);

	++myCounter;

	if (curDepth == 0) {
		return sef(&m);
	}

	curPlayerId = max == 1 ? myColor : 1-myColor;
	moves = m.moves(curPlayerId);

	if (moves.empty()) {
		return sef(&m);
	}

	for (pruning = false, scored = false, optScore=0, itMove = moves.begin(); !pruning &&  itMove != moves.end(); ++itMove) {
		score = searchRecurs(m, *itMove,curDepth-1, 1-max,borders);
		pruning = alphaBetaPruning(max, borders, score);

    if (scored == false) {
			optScore = score;
			scored = true;
			bestMove = *itMove;
			//sprintf(buffer,"%s[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",std::string(curDepth*2,' ').c_str(),itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
			//debugView->render(buffer);
		} else if ( (max == 1 && score > optScore) || (max == 0 && score < optScore) ) {
				optScore = score;
				bestMove = *itMove;
				//sprintf(buffer,"%s[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",std::string(curDepth*2,' ').c_str(),itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
				//debugView->render(buffer);
		}
	}


	//sprintf(buffer,"%s[BM] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d\n",std::string(curDepth*2,' ').c_str(),bestMove.pos1.myX+'a',myModel->getBoardSizeY() - bestMove.pos1.myY + '0',bestMove.pos2.myX+'a',myModel->getBoardSizeY() - bestMove.pos2.myY + '0',bestMove.effect,bestMove.type,bestMove.player,myModel->getFigureData(bestMove.figureId).letter,optScore);
	//debugView->render(buffer);

	if (myTurnsCounter > 500) {
		m.makeMove(bestMove);
		myBoardView->myModel = &m;
		myBoardView->render();
		myBoardView->highlight(bestMove.pos1,13);
		myBoardView->highlight(bestMove.pos2,11);
		//myBoardView->wait();
	}

	return optScore;
}

int AlphaBetaAIPlayer::sef(Model *m) const {
	FIGURES::const_iterator itFigure;
	int myScore, oppScore;
	for (myScore = 0, itFigure = m->getSetFigures(myColor).begin(); itFigure != m->getSetFigures(myColor).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			myScore += m->getFigureData(itFigure->id).weight;
		}
	}
	for (oppScore = 0, itFigure = m->getSetFigures(1-myColor).begin(); itFigure != m->getSetFigures(1-myColor).end(); ++itFigure ) {
		if (itFigure->captured == false) {
			oppScore += m->getFigureData(itFigure->id).weight;
		}
	}
	return myScore-oppScore;

}
