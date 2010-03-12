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

enum RuleType {	JUMP, SLIDE };
enum MoveType {	CAPTURE=1 , MOVE=2 , INPASSING=4 };
enum MoveEffect { LONGMOVE=1, CASTLE=2, EXPLOSION=4 };

enum PlayerCommand {NOTHING, TURN, SAVE, EXIT };
enum GameMessage {NONE, WRONG_MOVE, GOT_CHECK, SAVED};

struct Position {
	int myX;
	int myY;
	inline Position(int x=0, int y=0) : myX(x), myY(y) { }
	inline bool operator!=(const Position& pos) { return myX != pos.myX || myY!= pos.myY; }
	inline bool operator==(const Position& pos) { return myX == pos.myX && myY == pos.myY; }
};
struct Figure {
	int id;
	Position position;
	bool wasMoved;
	bool captured;
	inline Figure() { id=0; wasMoved = false; captured = false;}
};
struct Promoting {
	int horizontal;
	int figure;
	inline Promoting() { horizontal = 0; figure = 0; }

};
struct FigureData {
	std::string name;
	char letter;
	bool special;
	bool explosion;
	int weight;
	Promoting promoting[2];
	inline FigureData() { name=""; letter = 0; weight = 0; special=false; explosion=true; }
};
struct Move {
	int player;
	Position pos1;
	Position pos2;
	int type;
	int effect;
	int figureId;
	inline Move() { player = 0; type=0; effect = 0; figureId=0;}
};

struct MoveRule {
	int dx;
	int dy;
	RuleType ruleType;
	int moveType;
	int player;
	int limit;
	int moveEffect;
	inline MoveRule() { dx=0; dy=0; ruleType=SLIDE; moveType=0; player=0; limit=0; moveEffect=0;}
};
struct CastleRule {
	int dx;
	int dy;
	int player;
	Position kingCell;
	Position rookCellStart,rookCellEnd;
	inline CastleRule() { dx=0; dy=0; player=0;}
};
#endif /* SNOWWCHESS_H_ */
