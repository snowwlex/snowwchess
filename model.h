#ifndef _MODEL_H
#define _MODEL_H

#define BUFFER_SIZE 2
#define BOARD_SIZE 8

// ENUMS
enum FiguresName {
	ROOK    = 'R', //ladja
	KNIGHT  = 'H', // loshad
	BISHOP  = 'B', //slon
	QUEEN   = 'Q',
	KING    = 'K',
	PAWN    = 'P', //peshka
	SPACE   = ' ',
	BUFFER  = '-' 
};
enum PlayerColor { WHITE = 1, BLACK = 2,	ALL = 3 };
enum RuleType {	ONE_TURN, DIRECTION };
enum MoveType {	EAT = 1, MOVE = 2, move_type = 3};


//STRUCTURES
struct Position {
	int x;
	int y;

	inline Position(int _x=0, int _y=0) : x(_x), y(_y) { }
};
struct Figure {
	Position position;
	FiguresName name;

	inline Figure(Position pos, FiguresName fn) : position(pos), name(fn) { }
};
struct Move {
	Position square_pos;
	MoveType mode;

	inline Move(Position pos, MoveType move_type): square_pos(pos), mode(move_type) { }
};
struct MoveRule {
	int delta_x;
	int delta_y;
	RuleType rule_type;
	MoveType move_type;
	PlayerColor player;
	int from_line;
	
	inline MoveRule(int x, int y, RuleType rtype=DIRECTION, MoveType eat=(MoveType)(EAT+MOVE), PlayerColor pl=(PlayerColor)(WHITE+BLACK), int line=-1) :
	 delta_x(x), delta_y(y), rule_type(rtype), move_type(eat), player(pl), from_line(line) { }
};

//CLASSES
class Board {
	private:
		// 8x8 - поле, 
	    //+2 клетки с каждой стороны на буферную зону
		char myBoardArray[BOARD_SIZE+2*BUFFER_SIZE][BOARD_SIZE+2*BUFFER_SIZE]; 
	public:
		Board();
		void Set( std::vector<Figure> figure_set);
		char& operator() (int x,int y);
};
class Model {
	private:
		Board myBoard;
		std::vector<Figure> SetFigures[3];
		std::map<FiguresName, std::vector<MoveRule> > myMoveRules;
		std::vector<Figure>::iterator findFigure(std::vector<Figure> &figures_set , Position find_pos);
		std::vector<Figure>::iterator findFigure(std::vector<Figure> &figures_set , FiguresName figure);
	public:

		void Init();
		void Render(WINDOW *window);
		bool move(Position pos1, Position pos2);
		bool CanMove(Position pos1, Position pos2);
		std::vector< Move > moves(Position pos1);
		//bool is_win();


};

#endif
