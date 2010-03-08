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

	sprintf(buffer,"------------------------\n");	debugView->render(buffer);

	for (std::vector<Move>::iterator it=avMoves.begin(); it!=avMoves.end(); ++it) {
		sprintf(buffer,"[%c on %d:%d]",myRules->getFigureData(it->figureId).letter,it->pos2.myX,it->pos2.myY);	debugView->render(buffer);
	}
	sprintf(buffer,"\n"); debugView->render(buffer);

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
		avMoves = moves(opponent, *itFigure, false);
		for (itMove = avMoves.begin(); !check && itMove != avMoves.end(); ++itMove) {
			if (itMove->pos2 == itSpecialFigure->position) {
				check = true;
			}
		}
	}
	return check;
}

void Model::makeMoveInpassing(const Move& move) {
	myBoard(passant_figure.position.myX,passant_figure.position.myY) = 0;
	int opponent = 1 - move.player;
	std::vector<Figure>::iterator itOpponent = findFigure(opponent, passant_figure.position);
	if ( itOpponent != mySetFigures[opponent].end() ) {
		mySetFigures[opponent].erase(itOpponent); //
	}
}
void  Model::makeMoveCapture(const Move& move) {
	int opponent = 1 - move.player;
	std::vector<Figure>::iterator itOpponent = findFigure(opponent, move.pos2);
	if ( itOpponent != mySetFigures[opponent].end() ) {
		mySetFigures[opponent].erase(itOpponent); //
	}
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
	//sprintf(buffer,"Making castle %d:%d pl=%d\n",move.pos2.myX,move.pos2.myY,move.player);
	//debugView->render(buffer);

	Move rookMove;
	CastleRule castleRule = myRules->getCastleRule(move.pos2.myX,move.pos2.myY,move.player);
	rookMove.pos1 = castleRule.rookCellStart;
	rookMove.pos2 = castleRule.rookCellEnd;
	rookMove.player = move.player;
	std::vector<Figure>::iterator itRook = findFigure(move.player, castleRule.rookCellStart);
	rookMove.figureId = itRook->id;
	rookMove.type = MOVE;

	//sprintf(buffer,"[%c%c-%c%c fId=%d, pl=%d]\n",rookMove.pos1.myX+'a',myRules->getBoardSizeY() - rookMove.pos1.myY+'0',rookMove.pos2.myX+'a',myRules->getBoardSizeY() - rookMove.pos2.myY+'0',rookMove.figureId,rookMove.player);
	//debugView->render(buffer);
	makeMove(rookMove);
}

void Model::makeMove(Move move) {

	std::vector<Figure>::iterator itFigure, itOpponent;


	//sprintf(buffer,"Removing %c '%d:%d' on '%d:%d'\n",myRules->getFigureData(move.figureId).letter,move.pos1.myX,move.pos1.myY,move.pos2.myX,move.pos2.myY);
	//debug_view->render(buffer);

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


}

std::vector< Move > Model::moves(int player, const Figure& figure,  bool needCheck) {
	bool accepted;
	int curLimit;
	std::vector< Move > avMoves;
	Move move;
	std::vector<MoveRule>::iterator itRule;
	Position curPos;
	std::vector < MoveRule > curRules = myRules->getMoveRules(figure.id);

	//sprintf(buffer,"-----AV MOVES---------------\n");
	//debug_view->render(buffer);

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
	//sprintf(buffer,"[%c%c, %d %d, l=%d, eff=%d, type=%d,pl=%d, %s] ",move.pos1.myX+'a',myRules->getBoardSizeY() - move.pos1.myY + '0',moveRule.dx,moveRule.dy,moveRule.limit,moveRule.moveEffect,moveRule.moveType,moveRule.player,moveRule.ruleType==JUMP?"JUMP":"SLIDE");
	//debugView->render(buffer);
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
	//sprintf(buffer," %s\n",accepted==true?"is accepted":"");
	//debugView->render(buffer);
	return accepted;
}


bool Model::checkLongMove(MoveRule moveRule, const Figure& figure, Move& move) {
	if (figure.wasMoved == true) {
		return false;
	}
	move.type = MOVE;
	move.effect = LONGMOVE;
	return true;
}
bool Model::checkCastle(MoveRule moveRule, const Figure& figure, Move& move) {

	//sprintf(buffer,"[%c%c, %d %d, l=%d, eff=%d, type=%d,pl=%d, %s]\n",move.pos1.myX+'a',myRules->getBoardSizeY() - move.pos1.myY + '0',moveRule.dx,moveRule.dy,moveRule.limit,moveRule.moveEffect,moveRule.moveType,moveRule.player,moveRule.ruleType==JUMP?"JUMP":"SLIDE");
	//debugView->render(buffer);

	if (figure.wasMoved == true) {
		return false;
	}
	CastleRule castleRule = myRules->getCastleRule(move.pos2.myX,move.pos2.myY,move.player);
	std::vector<Figure>::iterator itRook = findFigure(move.player, castleRule.rookCellStart);
	if (itRook->wasMoved == true) {
		return false;
	}
	move.type = MOVE;
	move.effect = CASTLE;

	//sprintf(buffer," is accepted\n");
	//debugView->render(buffer);
	return true;
}

bool Model::checkCapture(MoveRule moveRule, const Figure& figure, Move& move) {
	std::vector<Figure>::iterator itDestFigure = findFigure(move.player, move.pos2);
	if ( itDestFigure != mySetFigures[move.player].end() ) {
		return false;
	}
	move.type = CAPTURE;
	return true;
}
bool Model::checkIfCheck(MoveRule moveRule, const Figure& figure, Move& move) {
	Model m = *this;
	m.makeMove(move);
	return m.isCheck(move.player);
}
bool Model::checkMove(MoveRule moveRule, const Figure& figure, Move& move)  {
	bool accepted;
	if (moveRule.moveEffect == LONGMOVE) {
		accepted = checkLongMove(moveRule, figure, move);
	} else if (moveRule.moveEffect == CASTLE) {
		accepted = checkCastle(moveRule, figure, move);
	} else {
		move.type = MOVE;
		accepted = true;
	}
	return accepted;
}
bool Model::checkInpassing(MoveRule moveRule, const Figure& figure, Move& move) {
	if (longmove != true || passant_cell != move.pos2) {
		return false;
	}
	move.type = INPASSING;
	return true;
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
	if (itFigure == mySetFigures[player].end()) {
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
		avMoves = moves(player, *itFigure);
		for (itMove = avMoves.begin(); itMove != avMoves.end(); ++itMove) {
			allMoves.push_back(*itMove);
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
		if (itFigure->position.myX == findPos.myX && itFigure->position.myY == findPos.myY)
			break;
	}
	return itFigure;

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


