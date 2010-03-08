/*
 * rules.cpp
 *
 *
 *      Author: snowwlex
 */
#include <ncurses.h>
#include <vector>
#include <map>
#include <string>

#include "snowwchess.h"
#include "rules.h"

#include "view.h" // для debig view

void Rules::setRulesName(std::string rulesName) {
	myRulesName = rulesName;
}
std::string Rules::getRulesName() const {
	return myRulesName;
}
void Rules::setBoardSize(int sizex, int sizey) {
	myBoardSizeX = sizex;
	myBoardSizeY = sizey;
}

void Rules::setFirstTurn(int firstTurnId) {
	myFirstTurn = firstTurnId;
}
int Rules::getFirstTurn() const {
	return myFirstTurn;
}

void Rules::setMoveRule(int figureId, const MoveRule& moverule) {
	myMoveRules[figureId].push_back(moverule);
}

void Rules::setInitFigure(int playerId, const Figure& figure) {
	myInitFigures[playerId].push_back(figure);
}
void Rules::setFigureData(int figureId, const FigureData& figureData) {
	myFiguresData[figureId] = figureData;
}
const std::vector<Figure>& Rules::getInitFigures(int playerId) const {
	return myInitFigures[playerId];
}
const FigureData& Rules::getFigureData(int figureId) {
	return myFiguresData[figureId];
}
const std::vector < MoveRule >& Rules::getMoveRules(int figureId) {
	return myMoveRules[figureId];
}

std::string Rules::getPlayerData(int playerId) {
	return myPlayersData[playerId];
}
int Rules::getSpecialFigure() const {
	return mySpecialFigure;
}
void Rules::setPlayerData(int playerId, std::string name) {
      myPlayersData[playerId] = name;
}
void Rules::setSpecialFigure(int figureId) {
       mySpecialFigure = figureId;
}
int Rules::getBoardSizeX() const {
	return myBoardSizeX;
}
int Rules::getBoardSizeY() const {
	return myBoardSizeY;
}

const CastleRule& Rules::getCastleRule(int x,int y, int player) const {
	for (std::vector<CastleRule>::const_iterator it=myCastlesRules.begin(); it != myCastlesRules.end(); ++it) {
		sprintf(buffer,"CastleRule: %d:%d =?= %d:%d pl=%d\n",x,y,it->kingCell.myX,it->kingCell.myY, it->player);
		debugView->render(buffer);
		if (Position(x,y) == it->kingCell  && player == it->player) {
			debugView->render("returning it\n");
			return *it;
		}
	}
	return *(myCastlesRules.begin());
}

void Rules::setCastleRule(const CastleRule& castlerule) {
	myCastlesRules.push_back(castlerule);
}
