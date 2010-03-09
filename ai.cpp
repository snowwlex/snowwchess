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

AIPlayer::AIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView): myColor(color), myModel(model),
					myBoardView(boardView), myUserView(userView) {
	srand ( time(0) );
	myDepth = 3;
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

	move = search();

	sprintf(buffer, "My move: [%c%c-%c%c (%s+%d)]\n ", move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect );
	myUserView->render(buffer);
	command = TURN;
	return command;
}

Move AIPlayer::search() {
	Move move;
	std::vector< Move > moves;
	std::vector < Move >::iterator itMove;

	int maxScore=0, score,i,max;
	bool scored;

	myCounter = 0;
	debugView->clear();

	moves = myModel->moves(myColor);
	for (scored = false, max = i = 0, itMove = moves.begin(); itMove != moves.end(); ++itMove, ++i) {
		++myCounter;
		score = searchRecurs(*myModel, *itMove, 1, 0);
		sprintf(buffer,"[%c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
		debugView->render(buffer);
		if (scored == false) {
			maxScore = score;
			scored = true;
			max = i;
		} else if (score > maxScore){
			maxScore = score;
			max = i;
		}
	}

	myBoardView->myModel = myModel;
	sprintf(buffer,"My counter = %d\n",myCounter); debugView->render(buffer);
	return moves[max];
}

int AIPlayer::searchRecurs(Model m, Move move, int curDepth, int max ) {
	std::vector< Move > moves;
	std::vector < Move >::iterator itMove;
	int curPlayerId;
	int optScore, score;
	bool scored;

	Move bestMove;


	m.makeMove(move);

	if (curDepth == myDepth) {
		return sef(&m);
	}

	curPlayerId = max == 1 ? myColor : 1-myColor;
	moves = m.moves(curPlayerId);

	if (moves.size() == 0) {
		return sef(&m)-100;
	}

	for (scored = false, optScore=0, itMove = moves.begin(); itMove != moves.end(); ++itMove) {
		++myCounter;
		score = searchRecurs(m, *itMove,curDepth+1, 1-max);

		if (scored == false) {
			optScore = score;
			scored = true;
			bestMove = *itMove;
			if (itMove->type == CAPTURE) optScore += 30;
			//sprintf(buffer,"%s[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",std::string(curDepth*2,' ').c_str(),itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
			//debugView->render(buffer);
		} else if ( (max == 1 && score > optScore) || (max == 0 && score < optScore) ) {
				optScore = score;
				if (itMove->type == CAPTURE) optScore += 30;
				bestMove = *itMove;
				//sprintf(buffer,"%s[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",std::string(curDepth*2,' ').c_str(),itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,score);
				//debugView->render(buffer);
		}
	}


	//sprintf(buffer,"%s[BM] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d\n",std::string(curDepth*2,' ').c_str(),bestMove.pos1.myX+'a',myModel->getBoardSizeY() - bestMove.pos1.myY + '0',bestMove.pos2.myX+'a',myModel->getBoardSizeY() - bestMove.pos2.myY + '0',bestMove.effect,bestMove.type,bestMove.player,myModel->getFigureData(bestMove.figureId).letter,optScore);
	//debugView->render(buffer);


	//m.makeMove(bestMove);
	//myBoardView->myModel = &m;
	//myBoardView->render();
	//myBoardView->highlight(bestMove.pos1,13);
	//myBoardView->highlight(bestMove.pos2,11);
	//myBoardView->wait();

	return optScore;
}

int AIPlayer::sef(Model *m) const {
	std::vector<Figure>::const_iterator itFigure;
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
