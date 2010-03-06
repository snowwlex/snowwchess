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

void Model::Board::Init(int sizex, int sizey) {
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

void Model::Board::Set(const Figure& figure) {
	operator()(figure.position.x,figure.position.y) = figure.id;
}
int& Model::Board::operator() (int x,int y) {
	if ( (x+myBufferSize)<0 || x>=(mySizeX+myBufferSize) ||
	     (y+myBufferSize)<0 || y>=(mySizeY+myBufferSize) ){
		char buffer[1024];
		sprintf(buffer,"Exception. Out of border: %d %d\n",x,y);
		debug_view->Render(buffer); debug_view->Wait();
		return myBoardArray[myBufferSize][myBufferSize];
	}

	return myBoardArray[x+myBufferSize][y+myBufferSize];
}


Model::Model(Rules* _myRules): myRules(_myRules) { en_passant = false;  }

void Model::Init(int mode) {
	myBoard.Init(myRules->GetBoardSizeX(),myRules->GetBoardSizeY());
	if (mode == 0) {
		myCurrentPlayer = myRules->GetFirstTurn();
		mySpecialFigure = myRules->GetSpecialFigure();

		mySetFigures[WHITE] = myRules->GetInitFigures(WHITE);
		mySetFigures[BLACK] = myRules->GetInitFigures(BLACK);
	}
	for (int i=0; i < 2; ++i) {
		for (std::vector<Figure>::iterator it = mySetFigures[i].begin(); it!=mySetFigures[i].end(); ++it) {
			myBoard.Set(*it);
		}
	}
}

void Model::SetFigure(int player_id, const Figure& figure) {
	mySetFigures[player_id].push_back(figure);
}

int Model::GetCurrentPlayer() const {
	return myCurrentPlayer;
}
int Model::GetBoard(int x, int y) {
	return myBoard(x,y);
}

void Model::SetCurrentPlayer(int player_id) {
	myCurrentPlayer = player_id;
}

int Model::GetBoardSizeX() const {
	return myRules->GetBoardSizeX();
}
int Model::GetBoardSizeY() const {
	return myRules->GetBoardSizeY();
}
std::string Model::GetRulesName() const {
	return myRules->GetRulesName();
}
GameStatus Model::GetGameStatus(int player) {
	std::vector< Move > av_moves;
	bool is_check;
	av_moves = Moves(player);

	char buffer[1024];
	sprintf(buffer,"------------------------\n");	debug_view->Render(buffer);

	for (std::vector<Move>::iterator it=av_moves.begin(); it!=av_moves.end(); ++it) {
		sprintf(buffer,"[%c on %d:%d]",myRules->GetFigureData(it->figure_id).letter,it->pos2.x,it->pos2.y);	debug_view->Render(buffer);
	}
	sprintf(buffer,"\n"); debug_view->Render(buffer);

	is_check = IsCheck(player);

	if (is_check) {
		sprintf(buffer,"Check!\n");	debug_view->Render(buffer);
	}

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
	std::vector<Figure>::iterator it_SpecialFigure;
	std::vector <Move>::iterator it_move;
	int opponent = player == WHITE ? BLACK : WHITE;
	it_SpecialFigure = findFigure(player, mySpecialFigure );
	for (is_check = false, it_figure = mySetFigures[opponent].begin(); !is_check &&  it_figure != mySetFigures[opponent].end(); ++it_figure) {
		av_moves = Moves(opponent, it_figure, false);
		for (it_move = av_moves.begin(); !is_check && it_move != av_moves.end(); ++it_move) {
			if (it_move->pos2.x == it_SpecialFigure->position.x &&  it_move->pos2.y == it_SpecialFigure->position.y) {
				is_check = true;
			}
		}
	}
	return is_check;
}
void Model::Remove(Move move) {

	std::vector<Figure>::iterator it_figure, it_opp;
	int opponent;

	char buffer[1024];
	sprintf(buffer,"Removing %c '%d:%d' on '%d:%d'\n",myRules->GetFigureData(move.figure_id).letter,move.pos1.x,move.pos1.y,move.pos2.x,move.pos2.y);
	debug_view->Render(buffer);

	it_figure = findFigure(move.player, move.pos1);

	if (move.type & EAT) {
		opponent = move.player == WHITE ? BLACK : WHITE;
		it_opp = findFigure(opponent, move.pos2);
		if ( it_opp != mySetFigures[opponent].end() ) {
			mySetFigures[opponent].erase(it_opp); //
		}
	}

	/*
	if (move.type & EN_PASSANT) {
		passant_cell = Position(move)
		Figure passant_figure =
		en_passant = true;
	}
	*/

	if (move.type & CASTLE) {
		Move rookmove;
		CastleRule cr = myRules->GetCastleRule(move.pos2.x,move.pos2.y,move.player);
		rookmove.pos1 = cr.rookcell_start;
		rookmove.pos2 = cr.rookcell_end;
		rookmove.player = move.player;
		std::vector<Figure>::iterator it_rook = findFigure(move.player, cr.rookcell_start);
		rookmove.figure_id = it_rook->id;
		rookmove.type = MOVE;

		Remove(rookmove);
	}

	myBoard(move.pos2.x,move.pos2.y) = myBoard(move.pos1.x,move.pos1.y);
	myBoard(move.pos1.x,move.pos1.y) = 0;

	it_figure->position.x = move.pos2.x;
	it_figure->position.y = move.pos2.y;
	it_figure->unmoved = false;
}

bool Model::CanMove(Move move, int* movetype_ptr) {

	bool accepted;
	std::vector< Move > av_moves;
	std::vector < Move >::iterator it;

	av_moves = Moves(move.player, move.pos1);
	for ( accepted = false, it = av_moves.begin() ; !accepted && it != av_moves.end(); ++it ) {
		if ( it->pos1.x == move.pos1.x && it->pos1.y == move.pos1.y && it->pos2.x == move.pos2.x && it->pos2.y == move.pos2.y && (it->type & move.type) ) {
			if (movetype_ptr != 0) *movetype_ptr = it->type;
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
	bool accepted,free;
	std::vector< Move > av_moves;
	Move move;
	int cur_limit;
	int dx,dy;
	std::vector < MoveRule >::iterator it_rule;
	std::vector<Figure>::iterator it_dest_figure;
	Position cur_pos, prev_pos;
	std::vector < MoveRule > curRules = myRules->GetMoveRules(it_figure->id);

	for ( it_rule=curRules.begin() ; it_rule != curRules.end(); ++it_rule ) {
		cur_limit = 0;
		if  ( ((it_rule->player+1) & (player+1)) ) 	{

			cur_pos = it_figure->position;

			dx = getDirection( it_rule->delta_x);
			dy = getDirection( it_rule->delta_y);

			do  {
				accepted = false;
				free = true;
				prev_pos = cur_pos;
				cur_pos.x += it_rule->delta_x;
				cur_pos.y += it_rule->delta_y;

				if (it_rule->rule_type != JUMP) {
					Position check_pos = prev_pos;
					do {
						check_pos.x += dx;
						check_pos.y += dy;
						if ( check_pos != cur_pos) {
							if ( myBoard(check_pos.x,check_pos.y) != 0 ) {
								free = false;
							}
						}

					} while (check_pos != cur_pos && free );
				}


				if (free && myBoard(cur_pos.x,cur_pos.y) != -1) {



					move.figure_id = it_figure->id;
					move.pos1 = it_figure->position;
					move.pos2 = cur_pos;
					move.player = player;
					++cur_limit;
					if ( myBoard(cur_pos.x,cur_pos.y) == 0 && (it_rule->move_type & MOVE) ) {
						if (it_rule->move_type & LONGMOVE) {
							if (it_figure->unmoved == true) {
								move.type = MOVE | LONGMOVE;
								accepted = true;
							}
						} else if (it_rule->move_type & CASTLE) {
							if (it_figure->unmoved == true) {
								CastleRule cr = myRules->GetCastleRule(cur_pos.x,cur_pos.y,player);
								std::vector<Figure>::iterator it_rook = findFigure(player, cr.rookcell_start);
								if (it_rook->unmoved == true) {
									accepted = true;
									move.type = MOVE | CASTLE;
								}
							}
						}
						else {
							move.type = MOVE;
							accepted = true;
						}
					}

					if ( myBoard(cur_pos.x,cur_pos.y) == 0 && (it_rule->move_type & ENPASSANT)) {

						accepted = false;

					}
					if ( myBoard(cur_pos.x,cur_pos.y) != 0 ) {
						it_dest_figure = findFigure(player, cur_pos);
						if (it_dest_figure == mySetFigures[player].end() && (it_rule->move_type & EAT) ) {
							move.type = EAT;
							accepted = true;
						}
					}
					if(check == true && accepted == true) {
						Model m = *this;
						m.Remove(move);
						if ( m.IsCheck(player) ) {
							accepted = false;
						}
					}

					if (accepted == true) {
						av_moves.push_back(move);
					}

				}
			} while (
					myBoard(cur_pos.x,cur_pos.y) == 0 &&
					it_rule->rule_type == DIRECTION &&
					(it_rule->limit == 0 || cur_limit < it_rule->limit )
					); }
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

int Model::getDirection(int dir) const {
	if (dir > 0) return 1;
	if (dir < 0) return -1;
	return 0;
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


