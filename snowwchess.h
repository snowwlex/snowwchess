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
	Position(int x=0, int y=0) : myX(x), myY(y) { }
	bool operator!=(const Position& pos) const{ return myX != pos.myX || myY!= pos.myY; }
	bool operator==(const Position& pos) const { return myX == pos.myX && myY == pos.myY; }
};
struct Figure {
	int id;
	Position position;
	bool wasMoved;
	bool captured;
	Figure() { id=0; wasMoved = false; captured = false;}
	bool operator<(const Figure& figure) const { return id<figure.id; }
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
	FigureData() { name=""; letter = 0; weight = 0; special=false; explosion=true; }
};
struct Move {
	int player;
	Position pos1;
	Position pos2;
	// !!!! взятая фигура (id)
	// значение на новом месте (id)
	// координата для взятой фигуры
	// фигуры для рокировки (указатели?)
	// координаты хода ладьи при рокировке
	int type;
	int effect;
	// int status;
	int figureId;
	Move() { player = 0; type=0; effect = 0; figureId=0;}
	bool operator<(const Move& move) const { return type<move.type; }
};

struct MoveRule {
	int dx;
	int dy;
	RuleType ruleType;
	int moveType;
	int player;
	int limit;
	int moveEffect;
	MoveRule() { dx=0; dy=0; ruleType=SLIDE; moveType=0; player=0; limit=0; moveEffect=0;}
};
struct CastleRule {
	int dx;
	int dy;
	int player;
	Position kingCell;
	Position rookCellStart,rookCellEnd;
	CastleRule() { dx=0; dy=0; player=0;}
};

typedef std::vector<Move> MOVES;
typedef std::vector<Figure> FIGURES;
typedef std::vector<MoveRule> MOVERULES;
typedef std::map<int,MOVERULES > FIGURES_RULES;
typedef	std::map<int,FigureData > FIGURES_DATA;
typedef	std::map<int, std::string> PLAYERS_DATA;
typedef	std::vector<CastleRule> CASTLERULES;

extern int randomizator;
extern FILE* statfile;

#endif /* SNOWWCHESS_H_ */
