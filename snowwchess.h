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
enum RuleType {	JUMP, DIRECTION, SPECIAL };
enum MoveType {	EAT = 1, MOVE = 2, EAT_MOVE = 3, }; //CHECK = 4, MATE = 8};
enum PlayerCommand {NOTHING, TURN, SAVE, EXIT };
enum GameMessage {NONE, WRONG_MOVE, GOT_CHECK, SAVED};

struct Position {
	int x;
	int y;
	inline Position(int _x=0, int _y=0) : x(_x), y(_y) { }
};
struct Figure {
	int id;
	Position position;
	inline Figure() { }
	inline Figure(int _id, Position _position) : id(_id), position(_position) { }
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
	// id of figure
	//promotion?
	// mate, chess and stalemate or nothing
	Position pos1;
	Position pos2;
	int type;
	inline Move(int _player, Position _pos1, Position _pos2,  int _type): player(_player), pos1(_pos1), pos2(_pos2), type(_type) { }
	inline Move() { }
};

struct MoveRule {
	int delta_x;
	int delta_y;
	RuleType rule_type;
	int move_type;
	int player;
	inline MoveRule(int _deltax, int _deltay, RuleType _ruletype=DIRECTION, int _movetype=(EAT | MOVE), int _player=(WHITE+BLACK+1)) :
	 delta_x(_deltax), delta_y(_deltay), rule_type(_ruletype), move_type(_movetype), player(_player) { }
};

#endif /* SNOWWCHESS_H_ */
