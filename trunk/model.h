/*
 * model.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_H_
#define MODEL_H_

class Rules;

class Model {
	private:

		class Board {
			private:
				int myBufferSize;
				int* myBoardArray;
				int mySizeX, mySizeY;
				int getCoordinates(int x,int y) const;
			public:
				Board(int buffer=4); // 4 - размер буферной зоны
				~Board();
				Board(const Board& board);
				Board& operator=(const Board& board);

				void init(int sizex, int sizey);
				void setFigureOnBoard(const Figure& figure);
				void setBoardCell(int x,int y,int value);
				int operator() (int x,int y) const;
		};

		/********************/
		// для взятия на проходе
		Position passant_cell;
		Figure passant_figure;
		bool longmove;
		/********************/

		Board myBoard;
		Rules* myRules;

		int myCurrentPlayer;
		int mySpecialFigure;
		FIGURES mySetFigures[2];

		MOVES movesFigure(int player, const Figure& figure,  bool needCheck=true) const;

		bool isCheck(int player) const;

		FIGURES::const_iterator findFigureById(int player, int figure) const;
		FIGURES::iterator getFigureByPosition(int player, Position findPos);
		bool isFigureOnPosition(Position findPos, FIGURES::const_iterator itFindFigure) const;

		int getDirection(int dir) const;

		bool checkIsFree(MoveRule moveRule, Position startPos) const;
		bool checkPosition(MoveRule moveRule, const Figure& figure, Move& move, bool needCheck) const;

		bool checkMove(MoveRule moveRule, const Figure& figure, Move& move) const;
		bool checkCapture(MoveRule moveRule, const Figure& figure, Move& move) const;
		bool checkInpassing(MoveRule moveRule, const Figure& figure, Move& move) const;
		bool checkIfCheck(MoveRule moveRule, const Figure& figure, Move& move) const;

		bool checkLongMoveEffect(MoveRule moveRule, const Figure& figure,  Move& move) const;
		bool checkCastleEffect(MoveRule moveRule, const Figure& figure, Move& move) const;
		bool checkExplosionEffect(MoveRule moveRule, const Figure& figure, Move& move) const;


		void makeMovePromotion(const Move& move);

		void makeMoveInpassing(const Move& move);
		void makeMoveCapture(const Move& move);

		void makeMoveEffectExplosion(const Move& move);
		void makeMoveEffectLongMove(const Move& move);
		void makeMoveEffectCastle(const Move& move);


	public:


		Model(Rules* _myRules);
		void init(int mode);

		void makeMove(Move move);
		bool canMove(Move& move) const;
		MOVES movesFromPosition(int player, Position pos1) const;
		MOVES allMoves(int player) const;

		int getBoardCell(int x, int y) const;
		int getBoardSizeX() const;
		int getBoardSizeY() const;
		std::string getRulesName() const;
		GameStatus getGameStatus(int player) const;
		const FIGURES& getSetFigures(int player) const;
		const FigureData& getFigureData(int figureId) const;
		int getCurrentPlayer() const;
		std::string getPlayerData(int playerId) const;

		void setFigure(int playerId, const Figure& figure);
		void setCurrentPlayer(int playerId);

		FIGURES::const_iterator findFigureByPosition(int player, Position findPos) const;



};
#endif /* MODEL_H_ */
