/*
 * snowwchess.h
 *
 *  Created on: 04.03.2010
 *      Author: snowwlex
 */

#ifndef SNOWWCHESS_H_
#define SNOWWCHESS_H_

enum GameStatus {USUAL, CHECK, MATE, STALEMATE };
enum PlayerColor { WHITE = 0, BLACK = 1, ALL = 2 };
enum RuleType {	JUMP, DIRECTION };
enum MoveType {	EAT = 1, MOVE = 2, LONGMOVE = 4, EN_PASSANT=8, CASTLE=16 }; //CHECK = 4, MATE = 8};
enum PlayerCommand {NOTHING, TURN, SAVE, EXIT };
enum GameMessage {NONE, WRONG_MOVE, GOT_CHECK, SAVED};

struct Position {
	int x;
	int y;
	inline Position(int _x=0, int _y=0) : x(_x), y(_y) { }
	inline bool operator!=(const Position& pos) { return x != pos.x || y!= pos.y; }
	inline bool operator==(const Position& pos) { return x == pos.x && y == pos.y; }
};
struct Figure {
	int id;
	Position position;
	bool unmoved;
	inline Figure() { unmoved = true;}
	inline Figure(int _id, Position _position, int _unmoved=true) : id(_id), position(_position), unmoved(_unmoved) { }
};
struct FigureData {
	std::string name;
	char letter;
	bool special;
	inline FigureData() { }
	inline FigureData(std::string _name, char _letter, bool _special): name(_name), letter(_letter), special(_special) { }
};
struct Move {
	int player;
	Position pos1;
	Position pos2;
	int type;
	int figure_id;
	inline Move(int _player, Position _pos1, Position _pos2,  int _type, int _figure_id): player(_player), pos1(_pos1), pos2(_pos2), type(_type), figure_id(_figure_id) { }
	inline Move() { }
};

struct MoveRule {
	int delta_x;
	int delta_y;
	RuleType rule_type;
	int move_type;
	int player;
	int limit;
	inline MoveRule(int _deltax, int _deltay, RuleType _ruletype=DIRECTION, int _movetype=(EAT | MOVE), int _player=(WHITE+BLACK+1), int _limit=0) :
	 delta_x(_deltax), delta_y(_deltay), rule_type(_ruletype), move_type(_movetype), player(_player), limit(_limit) { }
};
struct CastleRule {
	int delta_x;
	int delta_y;
	int player;
	Position kingcell;
	Position rookcell_start,rookcell_end;
};
#endif /* SNOWWCHESS_H_ */
