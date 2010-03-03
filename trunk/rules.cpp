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
#include "rules.h"


void Rules::SetBoardSize(int sizex, int sizey) {
	myBoardSizeX = sizex;
	myBoardSizeY = sizey;
}
void Rules::SetMoveRule(int figure_id, std::vector < MoveRule > _moverules) {
	myMoveRules[figure_id] = _moverules;
}

void Rules::SetInitFigures(int player_id, std::vector<Figure> figures) {
	myInitFigures[player_id] = figures;
}
void Rules::SetFiguresData(int figure_id, FigureData figuredata) {
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
