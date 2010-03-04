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

void Rules::SetRulesName(std::string rules_name) {
	myRulesName = rules_name;
}
std::string Rules::GetRulesName() const {
	return myRulesName;
}
void Rules::SetBoardSize(int sizex, int sizey) {
	myBoardSizeX = sizex;
	myBoardSizeY = sizey;
}

void Rules::SetFirstTurn(int first_turn_id) {
	myFirstTurn = first_turn_id;
}
int Rules::GetFirstTurn() const {
	return myFirstTurn;
}

void Rules::SetMoveRule(int figure_id, const std::vector < MoveRule >& _moverules) {
	myMoveRules[figure_id] = _moverules;
}

void Rules::SetInitFigures(int player_id, const std::vector<Figure>& figures) {
	myInitFigures[player_id] = figures;
}
void Rules::SetFigureData(int figure_id, const FigureData& figuredata) {
	myFiguresData[figure_id] = figuredata;
}
const std::vector<Figure>& Rules::GetInitFigures(int player_id) const {
	return myInitFigures[player_id];
}
const FigureData& Rules::GetFigureData(int figure_id) {
	return myFiguresData[figure_id];
}
const std::vector < MoveRule >& Rules::GetMoveRules(int figure_id) {
	return myMoveRules[figure_id];
}

std::string Rules::GetPlayerData(int player_id) {
	return myPlayersData[player_id];
}
int Rules::GetSpecialFigure() const {
	return mySpecialFigure;
}
void Rules::SetPlayerData(int player_id, std::string name) {
      myPlayersData[player_id] = name;
}
void Rules::SetSpecialFigure(int figure_id) {
       mySpecialFigure = figure_id;
}
int Rules::GetBoardSizeX() const {
	return myBoardSizeX;
}
int Rules::GetBoardSizeY() const {
	return myBoardSizeY;
}
