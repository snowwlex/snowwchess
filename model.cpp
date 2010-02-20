#include <ncurses.h>
#include "vector"
#include "string"
#include "map"
#include "model.h"

Board::Board() {
	for (int i=0; i<BOARD_SIZE+2*BUFFER_SIZE; ++i) {
		for (int j=0; j<BOARD_SIZE+2*BUFFER_SIZE; ++j) {
			myBoardArray[i][j] = BUFFER;
		}
	}	
	for (int i=0; i<BOARD_SIZE; ++i) {
		for (int j=0; j<BOARD_SIZE; ++j) {
			operator()(i,j) = SPACE;
		}
	}	
}

void Board::Set( std::vector<Figure> figure_set) {
	std::vector<Figure>::iterator it;
	for ( it=figure_set.begin() ; it != figure_set.end(); ++it ) {
		operator()(it->position.x,it->position.y) = it->name;
	}
}
char& Board::operator() (int x,int y) {
	return myBoardArray[x+BUFFER_SIZE][y+BUFFER_SIZE]; 
}




void Model::Init() {

	// в этом методе можно подключать внешний XML-файл для получения информации о:
	//1) какие фигуры участвуют
	//2) их расположение
	//3) правила ходов

	// Перечисляем фигуры и их места

	SetFigures[WHITE].push_back(Figure( Position(0,7), ROOK));
	SetFigures[WHITE].push_back(Figure( Position(7,7), ROOK));
	SetFigures[WHITE].push_back(Figure( Position(1,7), KNIGHT));
	SetFigures[WHITE].push_back(Figure( Position(6,7), KNIGHT));
	SetFigures[WHITE].push_back(Figure( Position(2,7), BISHOP));
	SetFigures[WHITE].push_back(Figure( Position(5,7), BISHOP));
	SetFigures[WHITE].push_back(Figure( Position(3,7), QUEEN));
	SetFigures[WHITE].push_back(Figure( Position(4,7), KING));
	for (int i=0; i < BOARD_SIZE; ++i) {
		SetFigures[WHITE].push_back(Figure(Position(i,6), PAWN));
	}
	SetFigures[BLACK].push_back(Figure( Position(0,0), ROOK));
	SetFigures[BLACK].push_back(Figure( Position(7,0), ROOK));
	SetFigures[BLACK].push_back(Figure( Position(1,0), KNIGHT));
	SetFigures[BLACK].push_back(Figure( Position(6,0), KNIGHT));
	SetFigures[BLACK].push_back(Figure( Position(2,0), BISHOP));
	SetFigures[BLACK].push_back(Figure( Position(5,0), BISHOP));
	SetFigures[BLACK].push_back(Figure( Position(3,0), QUEEN));
	SetFigures[BLACK].push_back(Figure( Position(4,0), KING));
	for (int i=0; i<BOARD_SIZE; ++i) {
		SetFigures[BLACK].push_back(Figure(Position(i,1), PAWN));
	}
	
	// СТАВИМ ФИГУРЫ НА ДОСКУ

	myBoard.Set(SetFigures[WHITE]);
	myBoard.Set(SetFigures[BLACK]);

	//УСТАНОВКА ПРАВИЛ
	std::vector < MoveRule > tmp_rule;

	// ROOK
	tmp_rule.clear();
	tmp_rule.push_back( MoveRule(1,0));
	tmp_rule.push_back( MoveRule(-1,0));
	tmp_rule.push_back( MoveRule(0,1));
	tmp_rule.push_back( MoveRule(0,-1));
	myMoveRules[ROOK] = tmp_rule;

	// KNIGHT
	tmp_rule.clear();
	tmp_rule.push_back( MoveRule(2,1,ONE_TURN));
	tmp_rule.push_back( MoveRule(2,-1,ONE_TURN));
	tmp_rule.push_back( MoveRule(-2,1,ONE_TURN));
	tmp_rule.push_back( MoveRule(-2,-1,ONE_TURN));
	
	tmp_rule.push_back( MoveRule(1,2,ONE_TURN));
	tmp_rule.push_back( MoveRule(-1,2,ONE_TURN));
	tmp_rule.push_back( MoveRule(1,-2,ONE_TURN));
	tmp_rule.push_back( MoveRule(-1,-2,ONE_TURN));
	myMoveRules[KNIGHT] = tmp_rule;

	// BISHOP
	tmp_rule.clear();
	tmp_rule.push_back( MoveRule(1,1));
	tmp_rule.push_back( MoveRule(1,-1));
	tmp_rule.push_back( MoveRule(-1,1));
	tmp_rule.push_back( MoveRule(-1,-1));
	myMoveRules[BISHOP] = tmp_rule;

	// QUEEN = BISHOP+ROOK
	tmp_rule.clear();
	tmp_rule.push_back( MoveRule(1,1));
	tmp_rule.push_back( MoveRule(1,-1));
	tmp_rule.push_back( MoveRule(-1,1));
	tmp_rule.push_back( MoveRule(-1,-1));

	tmp_rule.push_back( MoveRule(1,0));
	tmp_rule.push_back( MoveRule(-1,0));
	tmp_rule.push_back( MoveRule(0,1));
	tmp_rule.push_back( MoveRule(0,-1));
	myMoveRules[QUEEN] = tmp_rule;

	// KING
	tmp_rule.clear();
	tmp_rule.push_back( MoveRule(1,0, ONE_TURN));
	tmp_rule.push_back( MoveRule(-1,0, ONE_TURN));
	tmp_rule.push_back( MoveRule(0,1, ONE_TURN));
	tmp_rule.push_back( MoveRule(0,-1, ONE_TURN));
	tmp_rule.push_back( MoveRule(1,1, ONE_TURN));
	tmp_rule.push_back( MoveRule(-1,-1, ONE_TURN));
	tmp_rule.push_back( MoveRule(1,-1, ONE_TURN));
	tmp_rule.push_back( MoveRule(-1,1, ONE_TURN));
	myMoveRules[KING] = tmp_rule;

	// PAWN
	tmp_rule.clear();
	tmp_rule.push_back( MoveRule(0,1,ONE_TURN, MOVE, BLACK ));
	tmp_rule.push_back( MoveRule(0,2,ONE_TURN, MOVE, BLACK, 1 ));
	tmp_rule.push_back( MoveRule(1,1,ONE_TURN, EAT, BLACK));
	tmp_rule.push_back( MoveRule(-1,1,ONE_TURN, EAT, BLACK));
	
	tmp_rule.push_back( MoveRule(0,-1,ONE_TURN, MOVE, WHITE ));
	tmp_rule.push_back( MoveRule(0,-2,ONE_TURN, MOVE, WHITE, 6));
	tmp_rule.push_back( MoveRule(1,-1,ONE_TURN, EAT, WHITE));
	tmp_rule.push_back( MoveRule(-1,-1,ONE_TURN, EAT, WHITE));
	myMoveRules[PAWN] = tmp_rule;



}

void Model::Render(WINDOW *window) {

	int color;

	wprintw(window, "  ");	
	for (int i = 0; i < BOARD_SIZE; ++i) {
		wprintw(window, "%c", i+'a');
	}
	wprintw(window, "\n");

	for (int i = 0; i < BOARD_SIZE; ++i) {
		wprintw(window, "%d ", i+1);
		for (int j = 0; j < BOARD_SIZE; ++j) {
			color = ((i+j)%2 == 0) + 1 ;
			wattron(window, COLOR_PAIR(color) | A_BOLD);
			wprintw(window, "%c", SPACE); //myBoard(j,i));
			wattroff(window, COLOR_PAIR(color) | A_BOLD);
		}
		wprintw(window, " %d\n",i+1);
	}

	wprintw(window, "  ");	
	for (int i = 0; i < BOARD_SIZE; ++i) {
		wprintw(window, "%c", i+'a');
	}
	wprintw(window, "\n");

	//рендерим фигуры
	std::vector<Figure>::iterator it;
	
	for ( it=SetFigures[WHITE].begin(); it != SetFigures[WHITE].end(); ++it ) {
		color = ((it->position.x+it->position.y)%2 == 0) + 1 ;
		wattron(window, COLOR_PAIR(color) | A_BOLD);
		wmove(window,it->position.y+1,it->position.x+2);
		wprintw(window, "%c", it->name);
		wattroff(window, COLOR_PAIR(color) | A_BOLD);
	}
	for ( it=SetFigures[BLACK].begin() ; it != SetFigures[BLACK].end(); ++it ) {
		color = ((it->position.x+it->position.y)%2 == 0) + 1 ;
		color += 2; // для черных фигур
		wattron(window, COLOR_PAIR(color) | A_BOLD);
		wmove(window,it->position.y+1,it->position.x+2);
		wprintw(window, "%c", it->name);
		wattroff(window, COLOR_PAIR(color) | A_BOLD);
	}


}

bool Model::move(Position pos1, Position pos2) {
	std::vector<Figure>::iterator it, it_opp;
	PlayerColor player, opponent;
	if ( CanMove(pos1,pos2) == false)  {
		return false;
	}

	player = WHITE;
	it = findFigure(SetFigures[player], pos1);
	if (it == SetFigures[player].end()) {
		player = BLACK;
		it = findFigure(SetFigures[player], pos1);
		if (it == SetFigures[player].end()) {
			return false;
		}
	}
	
	if (myBoard(pos2.x,pos2.y) != SPACE) {
		opponent = player == WHITE ? BLACK : WHITE;
		it_opp = findFigure(SetFigures[opponent], pos2);		
		SetFigures[opponent].erase(it_opp);
	}
	myBoard(pos2.x,pos2.y) = myBoard(pos1.x,pos1.y);
	myBoard(pos1.x,pos1.y) = SPACE;
	it->position.x = pos2.x;
	it->position.y = pos2.y;
	return true;
}

bool Model::CanMove(Position pos1, Position pos2) {

	bool accepted = false;
	MoveType move_type;
	std::vector < MoveRule >::iterator it;
	std::vector<Figure>::iterator it_player;
	FiguresName figure;
    PlayerColor player;
	//определяем, чей ход
	player = WHITE;
	it_player = findFigure(SetFigures[player], pos1);
	if (it_player == SetFigures[player].end()) {
		player = BLACK;
		it_player = findFigure(SetFigures[player], pos1);
		if (it_player == SetFigures[player].end()) { // на исходной точке есть фигура?
			return false;
		}
	}

	figure = it_player->name;
	// на месте назначения - своя фигура?
	it_player = findFigure(SetFigures[player], pos2);
	if (it_player != SetFigures[player].end()) {
		return false;
	}
	
	// определяем тип хода 
	if (myBoard(pos2.x,pos2.y) != SPACE) {
		move_type = EAT;
	}
	else {
		move_type = MOVE;
	}

	// подгружаем правила для данной фигуры
	std::vector < MoveRule > curRules = myMoveRules[figure];
	
	//перебираем правила
	for ( it=curRules.begin() ; !accepted && it != curRules.end(); ++it ) {
		// базовая проверка, подходит ли правило
		if ( (it->player & player) && (it->move_type & move_type) && (it->from_line == -1 || it->from_line == pos1.y) ) 
		{ 
			Position cur_pos = pos1;
			do {
				cur_pos.x += it->delta_x;
				cur_pos.y += it->delta_y;
			} while (myBoard(cur_pos.x,cur_pos.y) == SPACE && !(cur_pos.x == pos2.x && cur_pos.y == pos2.y ) && it->rule_type == DIRECTION); 
			if (cur_pos.x == pos2.x && cur_pos.y == pos2.y) {
				accepted = true;
			}
		} 
	}
	return accepted;
}

std::vector< Move > Model::moves(Position pos1) {

	std::vector< Move > av_moves;
	std::vector < MoveRule >::iterator it;
	std::vector<Figure>::iterator it_player;
	Position cur_pos;
	PlayerColor player;

	//определяем, чей ход
	player = WHITE;
	it_player = findFigure(SetFigures[player], pos1);
	if (it_player == SetFigures[player].end()) {
		player = BLACK;
		it_player = findFigure(SetFigures[player], pos1);
		if (it_player == SetFigures[player].end()) {	
			return av_moves; // на исходной точке нет фигуры
		}
	}

	// подгружаем правила для данной фигуры
	std::vector < MoveRule > curRules = myMoveRules[it_player->name];
	//перебираем правила, возможные ходы записываем в av_moves
	for ( it=curRules.begin() ; it != curRules.end(); ++it ) {
		// базовая проверка на то, подходит ли правило
		if  ( (it->player & player)  && (it->from_line == -1 || it->from_line == pos1.y) ) 	{
			cur_pos = pos1;
			do  {
				cur_pos.x += it->delta_x;
				cur_pos.y += it->delta_y;
				// можно ли встать в эту точку?
				if (myBoard(cur_pos.x,cur_pos.y) != BUFFER) {
					if ( myBoard(cur_pos.x,cur_pos.y) == SPACE && (it->move_type & MOVE) )
						av_moves.push_back( Move(cur_pos, MOVE));
					if ( myBoard(cur_pos.x,cur_pos.y) != SPACE ) {
						it_player = findFigure(SetFigures[player], cur_pos);
						if (it_player == SetFigures[player].end() && (it->move_type & EAT) ) {
							av_moves.push_back( Move(cur_pos, EAT));
						}
					}			
				}				
			} while (myBoard(cur_pos.x,cur_pos.y) == SPACE && it->rule_type == DIRECTION);		
		}
	}
	return av_moves;
}

std::vector<Figure>::iterator Model::findFigure(std::vector<Figure> &figures_set , Position find_pos) {
	std::vector < Figure >::iterator it;
	for ( it=figures_set.begin() ; it != figures_set.end(); ++it ) {
		if (it->position.x == find_pos.x && it->position.y == find_pos.y)
			break;	
	}
	return it;

}

std::vector<Figure>::iterator Model::findFigure(std::vector<Figure> &figures_set , FiguresName figure) {
	std::vector < Figure >::iterator it;
	for ( it=figures_set.begin() ; it != figures_set.end(); ++it ) {
		if (it->name == figure)
			break;	
	}
	return it;

}

