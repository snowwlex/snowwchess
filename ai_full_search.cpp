/*
 * ai_full_search.cpp
 *
 *  Created on: 24.04.2010
 *      Author: snowwlex
 */

#include "ai.h"
//
//FullSearchAIPlayer::FullSearchAIPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView, int depth, int depth2):
//		AIPlayer(color,model,boardView,userView,depth,depth2) { }
//
//PlayerCommand FullSearchAIPlayer::makeTurn(Move& move, GameMessage message) {
//
//	PlayerCommand command;
//	int score;
//	time_t start,end;
//
//	switch(message) {
//		case GOT_CHECK:
//				myUserView->render("I'm gotta check!\n");
//				break;
//		default:
//				break;
//	}
//
//
//	++myTurnsCounter;
//	myCounter = 0;
//	srand ( myTurnsCounter*randomizator );
//	debugView->clear();
//
//	time(&start);
//	score = miniMaxSearch( move, myDepth, myColor, *myModel);
//	time(&end);
//	sprintf(buffer, "[US] My move #%d: [%c%c-%c%c (%s+%d)]\n",myTurnsCounter, move.pos1.myX+'a',myModel->getBoardSizeY() - move.pos1.myY+'0',move.pos2.myX+'a', myModel->getBoardSizeY() - move.pos2.myY+'0', move.type == CAPTURE ? "capture" : "move", move.effect ); myUserView->render(buffer); //myUserView->wait();
//	sprintf(buffer,"My counter = %d, myDepth = %d, turns = %d\n",myCounter, myDepth, myTurnsCounter); debugView->render(buffer);
//	sprintf(buffer,"Time for searching: %f\n",difftime(end,start)); debugView->render(buffer);
//	command = TURN;
//	return command;
//}
//
//
//int FullSearchAIPlayer::miniMaxSearch(Move& returnMove, int curDepth, int curPlayer, const Model& model) {
//	MOVES moves;
//	Move bestMove;
//	MOVES::iterator itMove;
//	int score, tmpScore;
//	bool scored;
//
//	++myCounter;
//
//	if (curDepth == 0) {
//		return sefMaterial(model,myColor);
//	}
//
//	moves = model.allMoves(curPlayer);
//
//	if (moves.empty()) {
//		return sefMaterial(model, myColor);
//	}
//
//	for (scored = false, score=0, itMove = moves.begin(); itMove != moves.end(); ++itMove) {
//		Model curModel = model;
//		curModel.makeMove(*itMove);
//		tmpScore = miniMaxSearch(bestMove,curDepth-1, 1-curPlayer, curModel);
//		if (curDepth == myDepth) {
//			sprintf(buffer,"[US] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,tmpScore); debugView->render(buffer);
//			//debugView->wait();
//		}
//
//		if (scored == false) {
//			score = tmpScore;
//			returnMove = *itMove;
//			scored = true;
//		} else {
//			if ( (curPlayer == myColor && tmpScore > score) || ( curPlayer != myColor && tmpScore < score) ) {
//				score = tmpScore;
//				returnMove = *itMove;
//			} else if (false && tmpScore == score && rand()%2 ) {
//				returnMove = *itMove;
//			}
//		}
//	}
//	return score;
//}
//
