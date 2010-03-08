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
				int** myBoardArray;
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
		std::vector<Figure> mySetFigures[2];

		std::vector<Move> moves(int player, const Figure& figure,  bool needCheck=true);

		bool isCheck(int player);

		std::vector<Figure>::iterator findFigure(int player, int figure);

		int getDirection(int dir) const;

		bool checkIsFree(MoveRule moveRule, Position startPos);
		bool checkPosition(MoveRule moveRule, const Figure& figure, Move& move, bool needCheck);
		bool checkLongMove(MoveRule moveRule, const Figure& figure,  Move& move);
		bool checkCastle(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkCapture(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkInpassing(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkMove(MoveRule moveRule, const Figure& figure, Move& move);
		bool checkIfCheck(MoveRule moveRule, const Figure& figure, Move& move);


		void makeMoveInpassing(const Move& move);
		void makeMoveCapture(const Move& move);
		void makeMoveEffectLongMove(const Move& move);
		void makeMoveEffectCastle(const Move& move);


	public:


		Model(Rules* _myRules);
		void init(int mode);

		void makeMove(Move move);
		bool canMove(Move& move);
		std::vector< Move > moves(int player, Position pos1);
		std::vector< Move > moves(int player);

		int getBoard(int x, int y);
		int getBoardSizeX() const;
		int getBoardSizeY() const;
		std::string getRulesName() const;
		GameStatus getGameStatus(int player);
		const std::vector<Figure>& getSetFigures(int player) const;
		const FigureData& getFigureData(int figureId) const;
		int getCurrentPlayer() const;

		void setFigure(int playerId, const Figure& figure);
		void setCurrentPlayer(int playerId);

		std::vector<Figure>::iterator findFigure(int player, Position findPos);



};
#endif /* MODEL_H_ */
