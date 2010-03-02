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

#include "rules.h"
#include "model.h"
#include "io.h"

Board::Board() {
	for (int i=0; i<BOARD_SIZE+2*BUFFER_SIZE; ++i) {
		for (int j=0; j<BOARD_SIZE+2*BUFFER_SIZE; ++j) {
			myBoardArray[i][j] = -1;
		}
	}
	for (int i=0; i<BOARD_SIZE; ++i) {
		for (int j=0; j<BOARD_SIZE; ++j) {
			operator()(i,j) = 0;
		}
	}
}

void Board::Set( std::vector<Figure> figure_set) {
	std::vector<Figure>::iterator it;
	for ( it=figure_set.begin() ; it != figure_set.end(); ++it ) {
		operator()(it->position.x,it->position.y) = it->id;
	}
}
int& Board::operator() (int x,int y) {
	return myBoardArray[x+BUFFER_SIZE][y+BUFFER_SIZE];
}


Model::Model(Rules* _myRules): myRules(_myRules) {  }

GameStatus Model::GetGameStatus(int player) {
	std::vector< Move > av_moves;
	bool is_check;
	av_moves = Moves(player);
	is_check = IsCheck(player);

	if ( av_moves.size() == 0 && is_check == true) {
		return MATE;
	} else if (av_moves.size() == 0) {
		return STALEMATE;
	} else if (is_check == true) {
		return CHECK;
	}
	return USUAL;
}


bool Model::IsCheck(int player) {
	bool is_check;
	std::vector<Move> av_moves;
	std::vector<Figure>::iterator it_figure;
	std::vector<Figure>::iterator it_KingFigure;
	std::vector <Move>::iterator it_move;
	int opponent = player == WHITE ? BLACK : WHITE;
	it_KingFigure = findFigure(player, 1 );
	for (is_check = false, it_figure = mySetFigures[opponent].begin(); !is_check &&  it_figure != mySetFigures[opponent].end(); ++it_figure) {
		av_moves = Moves(opponent, it_figure, false);
		for (it_move = av_moves.begin(); !is_check && it_move != av_moves.end(); ++it_move) {
			if (it_move->pos2.x == it_KingFigure->position.x &&  it_move->pos2.y == it_KingFigure->position.y) {
				is_check = true;
			}
		}
	}
	return is_check;
}
void Model::Remove(Move move) {

	std::vector<Figure>::iterator it_figure, it_opp;
	int opponent;

	it_figure = findFigure(move.player, move.pos1);

	if (move.type & EAT) {
		opponent = move.player == WHITE ? BLACK : WHITE;
		it_opp = findFigure(opponent, move.pos2);
		if ( it_opp != mySetFigures[opponent].end() ) {

			erased.position.x = it_opp->position.x;
			erased.position.y = it_opp->position.y;
			erased.id = it_opp->id;

			mySetFigures[opponent].erase(it_opp); //
		}
	}

	myBoard(move.pos2.x,move.pos2.y) = myBoard(move.pos1.x,move.pos1.y);
	myBoard(move.pos1.x,move.pos1.y) = 0;

	moved.position.x = move.pos1.x;
	moved.position.y = move.pos1.y;
	moved.id = it_figure->id;

	it_figure->position.x = move.pos2.x;
	it_figure->position.y = move.pos2.y;
}

void Model::Unmove(Move move) {

	std::vector<Figure>::iterator it_figure, it_opp;
	int opponent;

	it_figure = findFigure(move.player, move.pos2);

	if (move.type & EAT) {
		opponent = move.player == WHITE ? BLACK : WHITE;
		mySetFigures[opponent].push_back(erased);
		myBoard(move.pos2.x,move.pos2.y) = erased.id;
	}
	else {
		myBoard(move.pos2.x,move.pos2.y) = 0;
	}



	myBoard(move.pos1.x,move.pos1.y) = moved.id;

	it_figure->position.x = move.pos1.x;
	it_figure->position.y = move.pos1.y;

}

bool Model::CanMove(Move move) {

	bool accepted;
	std::vector< Move > av_moves;
	std::vector < Move >::iterator it;

	av_moves = Moves(move.player, move.pos1);
	for ( accepted = false, it = av_moves.begin() ; !accepted && it != av_moves.end(); ++it ) {
		if ( it->pos1.x == move.pos1.x && it->pos1.y == move.pos1.y && it->pos2.x == move.pos2.x && it->pos2.y == move.pos2.y && (it->type & move.type) ) {
			accepted = true;
		}
	}
	return accepted;
}

std::vector< Move > Model::Moves(int player, Position pos1) {

	std::vector< Move > av_moves;
	std::vector<Figure>::iterator it_figure;


	it_figure = findFigure(player, pos1);
	if (it_figure == mySetFigures[player].end()) {
		return av_moves; // на исходной точке нет фигуры
	}

	av_moves = Moves(player,it_figure);
	return av_moves;
}

std::vector< Move > Model::Moves(int player, std::vector<Figure>::iterator it_figure,  bool check) {
	bool accepted;
	std::vector< Move > av_moves;
	Move move;
	std::vector < MoveRule >::iterator it_rule;
	std::vector<Figure>::iterator it_player;
	Position cur_pos;
	std::vector < MoveRule > curRules = myRules->GetMoveRules(it_figure->id);
	for ( it_rule=curRules.begin() ; it_rule != curRules.end(); ++it_rule ) {
		if  ( ((it_rule->player+1) & (player+1))  && (it_rule->from_line == -1 || it_rule->from_line == it_figure->position.y) ) 	{
			cur_pos = it_figure->position;
			do  {
				accepted = false;
				cur_pos.x += it_rule->delta_x;
				cur_pos.y += it_rule->delta_y;
				if (myBoard(cur_pos.x,cur_pos.y) != -1) {
					move.pos1 = it_figure->position;
					move.pos2 = cur_pos;
					move.player = player;
					if ( myBoard(cur_pos.x,cur_pos.y) == 0 && (it_rule->move_type & MOVE) ) {
						move.type = MOVE;
						accepted = true;
					}
					if ( myBoard(cur_pos.x,cur_pos.y) != 0 ) {
						it_player = findFigure(player, cur_pos);
						if (it_player == mySetFigures[player].end() && (it_rule->move_type & EAT) ) {
							move.type = EAT;
							accepted = true;
						}
					}
					if(check == true && accepted == true) {
						//апдейт модели - совершаем ход
						Remove(move);
						if ( IsCheck(player) ) {
							accepted = false;
						}
						//делаем ход обратно
						Unmove(move);
					}
					if (accepted == true) {
						av_moves.push_back(move);
					}
				}
			} while (myBoard(cur_pos.x,cur_pos.y) == 0 && it_rule->rule_type == DIRECTION);
		}
	}
	return av_moves;
}
std::vector< Move > Model::Moves(int player) {

	std::vector< Move > av_moves, all_moves;
	std::vector<Figure>::iterator it_figure;
	std::vector<Move>::iterator it_move;

	for (it_figure = mySetFigures[player].begin();  it_figure != mySetFigures[player].end(); ++it_figure) {
		av_moves = Moves(player, it_figure);
		for (it_move = av_moves.begin(); it_move != av_moves.end(); ++it_move) {
			all_moves.push_back(*it_move);
		}
	}

	return all_moves;
}


std::vector<Figure>::iterator Model::findFigure(int player , Position find_pos) {

	std::vector < Figure >::iterator it_figure;

	for ( it_figure = mySetFigures[player].begin() ; it_figure != mySetFigures[player].end(); ++it_figure ) {
		if (it_figure->position.x == find_pos.x && it_figure->position.y == find_pos.y)
			break;
	}
	return it_figure;

}

std::vector<Figure>::iterator Model::findFigure(int player , int figure_id) {
	std::vector < Figure >::iterator it_figure;
	for ( it_figure = mySetFigures[player].begin() ; it_figure != mySetFigures[player].end(); ++it_figure ) {
		if (it_figure->id == figure_id)
			break;
	}
	return it_figure;

}

const std::vector<Figure>& Model::getSetFigures(int player) const {
	return mySetFigures[player];
}

const FigureData& Model::GetFigureData(int figure_id) const {
	return myRules->GetFigureData(figure_id);
}

void Model::SetInitFigures(int player) {
	SetFigures(player, myRules->GetInitFigures(player) );
}

void Model::SetFigures(int player_id, const std::vector<Figure>& setfigures) {
	mySetFigures[player_id] = setfigures;
	myBoard.Set(mySetFigures[player_id]);
}
