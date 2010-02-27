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
#include "model.h"

void Rules::SetModel(Model* _myModel) {
	myModel = _myModel;
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
const std::vector<Figure>& Rules::GetInitFigures(int player_id) {
	return myInitFigures[player_id];
}
const FigureData& Rules::GetFigureData(int figure_id) {
	return myFiguresData[figure_id];
}
bool Rules::IsCheck( int player ) {

	bool is_check;
	std::vector<Move> av_moves;
	std::vector<Figure>::iterator it_figure;
	std::vector<Figure>::iterator it_KingFigure;
	std::vector <Move>::iterator it_move;
	int opponent;

	opponent = WHITE ? BLACK : WHITE;
	it_KingFigure = myModel->findFigure(player, 1 );

	for (is_check = false, it_figure = myModel->mySetFigures[opponent].begin(); !is_check &&  it_figure != myModel->mySetFigures[opponent].end(); ++it_figure) {
		av_moves = Moves(opponent, it_figure, false);
		for (it_move = av_moves.begin(); !is_check && it_move != av_moves.end(); ++it_move) {
			if (it_move->pos2.x == it_KingFigure->position.x &&  it_move->pos2.y == it_KingFigure->position.y) {
				is_check = true;
			}
		}
	}

	return is_check;

}

/*
std::vector< Position > Rules::Eatable(PlayerColor player) {
	std::vector< Position> eatable;
	std::vector<Figure>::iterator it;
	std::vector< Move >::iterator it_moves;
	std::vector< Move > eat_moves;
	for (eatable.clear(), it = myModel->mySetFigures[player].begin(); it != myModel->mySetFigures[player].end(); ++it) {
		eat_moves = Moves(player, it->position);
		for (it_moves = eat_moves.begin(); it_moves != eat_moves.end(); ++it_moves) {
			eatable.push_back( it_moves->pos2 );
		}
	}
	return eatable;
}
*/

std::vector< Move > Rules::Moves(int player, std::vector<Figure>::iterator it_figure,  bool check) {

	bool accepted;

	std::vector< Move > av_moves;
	Move move;
	std::vector < MoveRule >::iterator it_rule;

	std::vector<Figure>::iterator it_player;
	Position cur_pos;

	std::vector < MoveRule > curRules = myMoveRules[it_figure->id];
	for ( it_rule=curRules.begin() ; it_rule != curRules.end(); ++it_rule ) {
		if  ( ((it_rule->player+1) & (player+1))  && (it_rule->from_line == -1 || it_rule->from_line == it_figure->position.y) ) 	{
			cur_pos = it_figure->position;
			do  {
				accepted = false;
				cur_pos.x += it_rule->delta_x;
				cur_pos.y += it_rule->delta_y;
				if (myModel->myBoard(cur_pos.x,cur_pos.y) != -1) {

					move.pos1 = it_figure->position;
					move.pos2 = cur_pos;
					move.player = player;
					if ( myModel->myBoard(cur_pos.x,cur_pos.y) == 0 && (it_rule->move_type & MOVE) ) {
						move.type = MOVE;
						accepted = true;
					}
					if ( myModel->myBoard(cur_pos.x,cur_pos.y) != 0 ) {
						it_player = myModel->findFigure(player, cur_pos);
						if (it_player == myModel->mySetFigures[player].end() && (it_rule->move_type & EAT) ) {
							move.type = EAT;
							accepted = true;
						}
					}

					if(check == true && accepted == true) {
						//апдейт модели - совершаем ход
						myModel->Remove(move);
						if ( IsCheck(player) ) {
							accepted = false;
						}
						//делаем ход обратно
						myModel->Unmove(move);
					}


					if (accepted == true) {
						av_moves.push_back(move);
					}



				}
			} while (myModel->myBoard(cur_pos.x,cur_pos.y) == 0 && it_rule->rule_type == DIRECTION);
		}
	}

	return av_moves;
}
