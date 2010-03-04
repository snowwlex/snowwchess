/*
 * rules.h
 *
 *
 *      Author: snowwlex
 */

#ifndef RULES_H_
#define RULES_H_

//ENUMS
enum GameStatus {USUAL, CHECK, MATE, STALEMATE };
enum PlayerColor { WHITE = 0, BLACK = 1, ALL = 2 };
enum RuleType {	JUMP, DIRECTION, SPECIAL };
enum MoveType {	EAT = 1, MOVE = 2, EAT_MOVE = 3, }; //CHECK = 4, MATE = 8};
//STRUCTURES
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
	Position pos1;
	Position pos2;
	MoveType type;

	inline Move(int _player, Position _pos1, Position _pos2,  MoveType _type): player(_player), pos1(_pos1), pos2(_pos2), type(_type) { }
	inline Move() { }
};

struct MoveRule {
	int delta_x;
	int delta_y;
	RuleType rule_type;
	MoveType move_type;
	int player;
	int from_line;

	inline MoveRule(int _deltax, int _deltay, RuleType _ruletype=DIRECTION, MoveType _movetype=(MoveType)(EAT + MOVE), int _player=(int)(WHITE+BLACK+1), int _fromline=-1) :
	 delta_x(_deltax), delta_y(_deltay), rule_type(_ruletype), move_type(_movetype), player(_player), from_line(_fromline) { }
};

class Rules {
	private:
		std::map<int, std::vector<MoveRule> > myMoveRules;
		std::map < int , FigureData > myFiguresData;
		std::map <int, std::string> myPlayersData;
		std::vector<Figure> myInitFigures[2];
		std::string myRulesName;
		int myFirstTurn;
		int mySpecialFigure;
		int myBoardSizeX, myBoardSizeY;
	public:

		const std::vector < MoveRule >& GetMoveRules(int figure_id);
		const FigureData& GetFigureData(int figure_id);
		const std::vector<Figure>& GetInitFigures(int player_id) const;
		std::string GetRulesName() const;
		int GetFirstTurn() const;
		std::string GetPlayerData(int player_id);
		int GetSpecialFigure() const;
		int GetBoardSizeX() const;
		int GetBoardSizeY() const;

		void SetPlayerData(int player_id, std::string name);
		void SetSpecialFigure(int figure_id);
		void SetFirstTurn(int first_turn_id);
		void SetBoardSize(int sizex, int sizey);
		void SetRulesName(std::string rules_name);
		void SetInitFigures(int player_id, const std::vector<Figure>& figures);
		void SetMoveRule(int figure_id, const std::vector < MoveRule >&  _moverules);
		void SetFigureData(int figure_id, const FigureData& figuredata);


};

#endif /* RULES_H_ */
