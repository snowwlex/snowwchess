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
			public:
				Board(int buffer=4); // 4 - размер буферной зоны
				~Board();
				Board(const Board& board);
				Board& operator=(const Board& board);

				void init(int sizex, int sizey);
				void set(const Figure& figure);
				int& operator() (int x,int y);
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

		MOVES moves(int player, const Figure& figure,  bool needCheck=true);

		bool isCheck(int player);

		FIGURES::iterator findFigure(int player, int figure);
		bool isFigureOnPosition(Position findPos, FIGURES::iterator& itFindFigure);

		int getDirection(int dir) const;

		bool checkIsFree(MoveRule moveRule, Position startPos);
		bool checkPosition(MoveRule moveRule, const Figure& figure, Move& move, bool needCheck);

		bool checkMove(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkCapture(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkInpassing(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkIfCheck(MoveRule moveRule, const Figure& figure, Move& move);

		bool checkLongMoveEffect(MoveRule moveRule, const Figure& figure,  Move& move);
		bool checkCastleEffect(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkExplosionEffect(MoveRule moveRule, const Figure& figure, Move& move);


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
		bool canMove(Move& move);
		MOVES moves(int player, Position pos1);
		MOVES moves(int player);

		int getBoard(int x, int y);
		int getBoardSizeX() const;
		int getBoardSizeY() const;
		std::string getRulesName() const;
		GameStatus getGameStatus(int player);
		const FIGURES& getSetFigures(int player) const;
		const FigureData& getFigureData(int figureId) const;
		int getCurrentPlayer() const;
		std::string getPlayerData(int playerId) const;

		void setFigure(int playerId, const Figure& figure);
		void setCurrentPlayer(int playerId);

		FIGURES::iterator findFigure(int player, Position findPos);



};
#endif /* MODEL_H_ */