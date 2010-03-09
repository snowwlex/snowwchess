/*
 * model.cpp
 *
 *
 *      Author: snowwlex
 */
#include <ncurses.h>
#include <vector>
#include <string>
#include <map>

#include "snowwchess.h"
#include "rules.h"
#include "model.h"

#include "view.h" //for debug_view

Model::Board::Board(int buffer): myBufferSize(buffer), myBoardArray(0)  { }

void Model::Board::init(int sizex, int sizey) {
	mySizeX = sizex;
	mySizeY = sizey;
	myBoardArray = new int*[mySizeX+2*myBufferSize];
	for (int i=0; i < mySizeX+2*myBufferSize; ++i) {
		myBoardArray[i] = new int[mySizeY+2*myBufferSize];
	}
	for (int i=0; i<(mySizeX+2*myBufferSize); ++i) {
		for (int j=0; j<(mySizeY+2*myBufferSize); ++j) {
			myBoardArray[i][j] = -1;
		}
	}
	for (int i=0; i<mySizeX; ++i) {
		for (int j=0; j<mySizeY; ++j) {
			operator()(i,j) = 0;
		}
	}
}
Model::Board::~Board() {
	if (myBoardArray != 0) {
		for (int i=0; i < (mySizeX+2*myBufferSize); ++i) {
			delete[] myBoardArray[i];
		}
		delete[] myBoardArray;
	}
}

Model::Board::Board(const Board& board) {
	myBufferSize=board.myBufferSize;
	mySizeX=board.mySizeX;
	mySizeY=board.mySizeY;
	myBoardArray = new int*[mySizeX+2*myBufferSize];
	for (int i=0; i < mySizeX+2*myBufferSize; ++i) {
		myBoardArray[i] = new int[mySizeY+2*myBufferSize];
	}
	for (int i=0; i<(mySizeX+2*myBufferSize); ++i) {
		for (int j=0; j<(mySizeY+2*myBufferSize); ++j) {
			myBoardArray[i][j] = board.myBoardArray[i][j];
		}
	}
}

Model::Board& Model::Board::operator=(const Board& board) {
	if (myBoardArray != 0) {
		for (int i=0; i < (mySizeX+2*myBufferSize); ++i) {
			delete[] myBoardArray[i];
		}
		delete[] myBoardArray;
	}
	myBufferSize=board.myBufferSize;
	mySizeX=board.mySizeX;
	mySizeY=board.mySizeY;
	myBoardArray = new int*[mySizeX+2*myBufferSize];
	for (int i=0; i < mySizeX+2*myBufferSize; ++i) {
		myBoardArray[i] = new int[mySizeY+2*myBufferSize];
	}
	for (int i=0; i<(mySizeX+2*myBufferSize); ++i) {
		for (int j=0; j<(mySizeY+2*myBufferSize); ++j) {
			myBoardArray[i][j] = board.myBoardArray[i][j];
		}
	}
	return *this;
}

void Model::Board::set(const Figure& figure) {
	if ( (figure.position.myX+myBufferSize)<0 || figure.position.myX>=(mySizeX+myBufferSize) ||
		 (figure.position.myY+myBufferSize)<0 || figure.position.myY>=(mySizeY+myBufferSize) ){
			sprintf(buffer,"Exception. Trying to put figure out of border: %d %d\n",figure.position.myX,figure.position.myY);
			debugView->render(buffer); debugView->wait();
			return;
	}
	operator()(figure.position.myX,figure.position.myY) = figure.id;
}
int& Model::Board::operator() (int x,int y) {
	if ( (x+myBufferSize)<0 || x>=(mySizeX+myBufferSize) ||
	     (y+myBufferSize)<0 || y>=(mySizeY+myBufferSize) ){

		sprintf(buffer,"Exception. Out of border: %d %d\n",x,y);
		debugView->render(buffer); debugView->wait();

		return myBoardArray[myBufferSize][myBufferSize];
	}

	return myBoardArray[x+myBufferSize][y+myBufferSize];
}


Model::Model(Rules* _myRules): myRules(_myRules) { longmove = false;  }

void Model::init(int mode) {
	myBoard.init(myRules->getBoardSizeX(),myRules->getBoardSizeY());
	if (mode == 0) {
		myCurrentPlayer = myRules->getFirstTurn();
		mySpecialFigure = myRules->getSpecialFigure();

		mySetFigures[WHITE] = myRules->getInitFigures(WHITE);
		mySetFigures[BLACK] = myRules->getInitFigures(BLACK);
	}
	for (int i=0; i < 2; ++i) {
		for (std::vector<Figure>::iterator it = mySetFigures[i].begin(); it!=mySetFigures[i].end(); ++it) {
			myBoard.set(*it);
		}
	}
}

void Model::setFigure(int playerId, const Figure& figure) {
	mySetFigures[playerId].push_back(figure);
}

int Model::getCurrentPlayer() const {
	return myCurrentPlayer;
}
int Model::getBoard(int x, int y) {
	return myBoard(x,y);
}

void Model::setCurrentPlayer(int playerId) {
	myCurrentPlayer = playerId;
}

int Model::getBoardSizeX() const {
	return myRules->getBoardSizeX();
}
int Model::getBoardSizeY() const {
	return myRules->getBoardSizeY();
}
std::string Model::getRulesName() const {
	return myRules->getRulesName();
}
GameStatus Model::getGameStatus(int player) {
	std::vector< Move > avMoves;
	bool check;
	avMoves = moves(player);
	check = isCheck(player);

	if (check) {
		sprintf(buffer,"Check!\n");	debugView->render(buffer);
	}

	if ( avMoves.size() == 0 && check == true) {
		return MATE;
	} else if (avMoves.size() == 0) {
		return STALEMATE;
	} else if (check == true) {
		return CHECK;
	}
	return USUAL;
}


bool Model::isCheck(int player) {
	bool check;
	std::vector<Move> avMoves;
	std::vector<Figure>::iterator itFigure;
	std::vector<Figure>::iterator itSpecialFigure;
	std::vector <Move>::iterator itMove;
	int opponent = 1-player;
	itSpecialFigure = findFigure(player, mySpecialFigure );
	for (check = false, itFigure = mySetFigures[opponent].begin(); !check &&  itFigure != mySetFigures[opponent].end(); ++itFigure) {
		if (itFigure->captured == false) {
			avMoves = moves(opponent, *itFigure, false);
			for (itMove = avMoves.begin(); !check && itMove != avMoves.end(); ++itMove) {
				if (itMove->pos2 == itSpecialFigure->position) {
					check = true;
				}
			}
		}
	}
	return check;
}

void Model::makeMoveInpassing(const Move& move) {
	myBoard(passant_figure.position.myX,passant_figure.position.myY) = 0;
	int opponent = 1 - move.player;
	std::vector<Figure>::iterator itOpponent = findFigure(opponent, passant_figure.position);
	itOpponent->captured = true;
}
void  Model::makeMoveCapture(const Move& move) {
	int opponent = 1 - move.player;
	std::vector<Figure>::iterator itOpponent = findFigure(opponent, move.pos2);
	itOpponent->captured = true;
}

void Model::makeMoveEffectLongMove(const Move& move) {
	longmove = true;
	if (move.player == WHITE) {
		passant_cell.myX = move.pos2.myX;
		passant_cell.myY = move.pos2.myY+1;
	} else {
		passant_cell.myX = move.pos2.myX;
		passant_cell.myY = move.pos2.myY-1;
	}
	 passant_figure.id = move.figureId;
	 passant_figure.position = move.pos2;
}

void Model::makeMoveEffectCastle(const Move& move) {
	Move rookMove;
	CastleRule castleRule = myRules->getCastleRule(move.pos2.myX,move.pos2.myY,move.player);
	rookMove.pos1 = castleRule.rookCellStart;
	rookMove.pos2 = castleRule.rookCellEnd;
	rookMove.player = move.player;
	std::vector<Figure>::iterator itRook = findFigure(move.player, castleRule.rookCellStart);
	rookMove.figureId = itRook->id;
	rookMove.type = MOVE;
	makeMove(rookMove);
}

void Model::makeMoveEffectExplosion(const Move& move) {
	std::vector<Figure>::iterator itFigure;
	Position curPos;
	for (int i=-1; i<2; ++i) {
		for (int j=-1; j<2; ++j) {
			curPos.myX = move.pos2.myX + i;
			curPos.myY = move.pos2.myY + j;
			if ( isFigureOnPosition(curPos, itFigure) && ( curPos == move.pos2 || getFigureData(itFigure->id).explosion == true) ) {
					itFigure->captured = true;
					myBoard(curPos.myX,curPos.myY) = 0;
			}
		}
	}

}
void Model::makeMove(Move move) {

	std::vector<Figure>::iterator itFigure, itOpponent;


	itFigure = findFigure(move.player, move.pos1);

	if (move.type ==INPASSING) {
		makeMoveInpassing(move);
	} else if (move.type == CAPTURE) {

		makeMoveCapture(move);
	}


	myBoard(move.pos2.myX,move.pos2.myY) = myBoard(move.pos1.myX,move.pos1.myY);
	myBoard(move.pos1.myX,move.pos1.myY) = 0;

	itFigure->position =  move.pos2;
	itFigure->wasMoved = true;

	if (move.effect == LONGMOVE) {
		makeMoveEffectLongMove(move);
	} else {
		longmove = false;
	}

	if (move.effect == CASTLE) {
		makeMoveEffectCastle(move);
	}

	if (move.effect == EXPLOSION) {
		makeMoveEffectExplosion(move);
	}


}

std::vector< Move > Model::moves(int player, const Figure& figure,  bool needCheck) {
	bool accepted;
	int curLimit;
	std::vector< Move > avMoves;
	Move move;
	std::vector<MoveRule>::iterator itRule;
	Position curPos;
	std::vector < MoveRule > curRules = myRules->getMoveRules(figure.id);

	for ( itRule=curRules.begin() ; itRule != curRules.end(); ++itRule ) {
		if  ( itRule->player == ALL || itRule->player == player ) 	{
			if (itRule->ruleType == JUMP) {
				accepted = false;
				move.pos1 = figure.position;
				move.pos2.myX = move.pos1.myX + itRule->dx;
				move.pos2.myY = move.pos1.myY + itRule->dy;
				move.player = player;
				accepted = checkPosition(*itRule,figure,move,needCheck);
				if (accepted == true) {
					//sprintf(buffer,"[JUMP  %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c']\n",move.pos1.myX+'a',myRules->getBoardSizeY() - move.pos1.myY + '0',move.pos2.myX+'a',myRules->getBoardSizeY() - move.pos2.myY + '0',move.effect,move.type,move.player,getFigureData(move.figureId).letter);
					//if (needCheck == true) debugView->render(buffer);
					avMoves.push_back(move);
				}
			} else if (itRule->ruleType == SLIDE) {
				curLimit = 0;
				move.pos1 = figure.position;
				move.pos2 = move.pos1;
				do  {
					accepted = false;

					if (checkIsFree(*itRule, move.pos2) == true) {
						move.pos2.myX += itRule->dx;
						move.pos2.myY += itRule->dy;
						move.player = player;
						accepted = checkPosition(*itRule,figure,move,needCheck);
						if (accepted == true) {
							//sprintf(buffer,"[SLIDE %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c']\n",move.pos1.myX+'a',myRules->getBoardSizeY() - move.pos1.myY + '0',move.pos2.myX+'a',myRules->getBoardSizeY() - move.pos2.myY + '0',move.effect,move.type,move.player,getFigureData(move.figureId).letter);
							//if (needCheck == true) debugView->render(buffer);
							avMoves.push_back(move);
						}
						++curLimit;
					}
				} while (myBoard(move.pos2.myX,move.pos2.myY) == 0 && (itRule->limit == 0 || curLimit < itRule->limit ) );
			}
		}
	}
	return avMoves;
}

bool Model::checkIsFree(MoveRule moveRule, Position curPos) {
	bool isFree = true;
	Position endPos;
	int dx,dy;
	endPos = curPos;
	endPos.myX += moveRule.dx;
	endPos.myY += moveRule.dy;
	dx = getDirection(moveRule.dx);
	dy = getDirection(moveRule.dy);
	do {
		curPos.myX += dx;
		curPos.myY += dy;
		if ( curPos != endPos &&  myBoard(curPos.myX,curPos.myY) != 0 ) {
				isFree = false;
		}
	} while (curPos != endPos && isFree == true );
	return isFree;
}



bool Model::checkPosition(MoveRule moveRule, const Figure& figure, Move& move, bool needCheck) {

	if (myBoard(move.pos2.myX,move.pos2.myY) == -1) {
		return false;
	}
	Position curPos = move.pos2;
	bool accepted = false;

	move.figureId = figure.id;



	if ( myBoard(move.pos2.myX,move.pos2.myY) == 0 && (moveRule.moveType & MOVE) ) {
		accepted = checkMove(moveRule, figure, move);
	} else if ( myBoard(curPos.myX,curPos.myY) > 0 && (moveRule.moveType & CAPTURE) ) {
		accepted = checkCapture(moveRule, figure, move);
	} else	if ( myBoard(curPos.myX,curPos.myY) == 0 && (moveRule.moveType == INPASSING)) {
		accepted = checkInpassing(moveRule, figure, move);
	}
	if (accepted == true && needCheck == true) {
		accepted = !checkIfCheck(moveRule, figure, move);
	}
	return accepted;
}

bool Model::checkExplosionEffect(MoveRule moveRule, const Figure& figure, Move& move) {
	move.effect = EXPLOSION;
	return true;
}
bool Model::checkLongMoveEffect(MoveRule moveRule, const Figure& figure, Move& move) {
	if (figure.wasMoved == true) {
		return false;
	}
	move.effect = LONGMOVE;
	return true;
}
bool Model::checkCastleEffect(MoveRule moveRule, const Figure& figure, Move& move) {

	if (figure.wasMoved == true) {
		return false;
	}
	CastleRule castleRule = myRules->getCastleRule(move.pos2.myX,move.pos2.myY,move.player);
	std::vector<Figure>::iterator itRook = findFigure(move.player, castleRule.rookCellStart);
	if (itRook->wasMoved == true || itRook->captured == true) {
		return false;
	}
	move.effect = CASTLE;
	return true;
}

bool Model::checkCapture(MoveRule moveRule, const Figure& figure, Move& move) {
	bool accepted;
	std::vector<Figure>::iterator itDestFigure = findFigure(move.player, move.pos2);
	if ( itDestFigure != mySetFigures[move.player].end() ) {
		return false;
	}
	move.type = CAPTURE;
	move.effect = 0;
	if (moveRule.moveEffect == EXPLOSION) {
		accepted = checkExplosionEffect(moveRule,figure,move);
	}
	else {
		accepted = true;
	}
	return accepted;
}
bool Model::checkIfCheck(MoveRule moveRule, const Figure& figure, Move& move) {
	Model m = *this;
	m.makeMove(move);
	return m.isCheck(move.player);
}
bool Model::checkMove(MoveRule moveRule, const Figure& figure, Move& move)  {
	bool accepted;
	move.type = MOVE;
	move.effect = 0;
	if (moveRule.moveEffect == LONGMOVE) {
		accepted = checkLongMoveEffect(moveRule, figure, move);
	} else if (moveRule.moveEffect == CASTLE) {
		accepted = checkCastleEffect(moveRule, figure, move);
	} else {
		accepted = true;
	}
	return accepted;
}
bool Model::checkInpassing(MoveRule moveRule, const Figure& figure, Move& move) {
	bool accepted;
	if (longmove != true || passant_cell != move.pos2) {
		return false;
	}
	move.type = INPASSING;
	move.effect = 0;
	if (moveRule.moveEffect == EXPLOSION) {
		accepted = checkExplosionEffect(moveRule,figure,move);
	}
	else {
		accepted = true;
	}
	return accepted;
}

bool Model::canMove(Move& move) {

	bool accepted;
	std::vector< Move > avMoves;
	std::vector < Move >::iterator itMove;

	avMoves = moves(move.player, move.pos1);
	for ( accepted = false, itMove = avMoves.begin() ; !accepted && itMove != avMoves.end(); ++itMove ) {
		if ( itMove->pos1 == move.pos1 && itMove->pos2 == move.pos2) { // && (it->type & move.type) ) {
			move.effect = itMove->effect;
			move.type = itMove->type;
			accepted = true;
		}
	}
	return accepted;
}

std::vector< Move > Model::moves(int player, Position pos1) {

	std::vector< Move > avMoves;
	std::vector<Figure>::iterator itFigure;

	itFigure = findFigure(player, pos1);
	if (itFigure == mySetFigures[player].end() || itFigure->captured == true) {
		return avMoves;
	}

	avMoves = moves(player,*itFigure);
	return avMoves;
}

std::vector< Move > Model::moves(int player) {

	std::vector< Move > avMoves, allMoves;
	std::vector<Figure>::iterator itFigure;
	std::vector<Move>::iterator itMove;
	for (itFigure = mySetFigures[player].begin();  itFigure != mySetFigures[player].end(); ++itFigure) {
		if (itFigure->captured == false) {
			avMoves = moves(player, *itFigure);
			for (itMove = avMoves.begin(); itMove != avMoves.end(); ++itMove) {
				allMoves.push_back(*itMove);
			}
		}
	}
	return allMoves;
}

int Model::getDirection(int dir) const {
	if (dir > 0) return 1;
	if (dir < 0) return -1;
	return 0;
}

std::vector<Figure>::iterator Model::findFigure(int player , Position findPos) {

	std::vector < Figure >::iterator itFigure;

	for ( itFigure = mySetFigures[player].begin() ; itFigure != mySetFigures[player].end(); ++itFigure ) {
		if (itFigure->captured == false && itFigure->position == findPos)
			break;
	}
	return itFigure;

}

bool Model::isFigureOnPosition(Position findPos, std::vector<Figure>::iterator& itFindFigure) {
	std::vector < Figure >::iterator itFigure;

	for (int i=0; i<2; ++i) {
		for ( itFigure = mySetFigures[i].begin() ; itFigure != mySetFigures[i].end(); ++itFigure ) {
			if (itFigure->captured == false && itFigure->position == findPos) {
				itFindFigure = itFigure;
				return true;
			}
		}
	}
	return false;
}

std::vector<Figure>::iterator Model::findFigure(int player , int figureId) {
	std::vector < Figure >::iterator itFigure;
	for ( itFigure = mySetFigures[player].begin() ; itFigure != mySetFigures[player].end(); ++itFigure ) {
		if (itFigure->id == figureId)
			break;
	}
	return itFigure;
}

const std::vector<Figure>& Model::getSetFigures(int player) const {
	return mySetFigures[player];
}

const FigureData& Model::getFigureData(int figureId) const {
	return myRules->getFigureData(figureId);
}

std::string Model::getPlayerData(int playerId) const {
	return myRules->getPlayerData(playerId);
}


