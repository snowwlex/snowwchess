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

				void Init(int sizex, int sizey);
				void Set(const Figure& figure);
				int& operator() (int x,int y);
		};

		// для en passant хода
		Position passant_cell;
		Figure passant_figure;
		bool en_passant;

		Board myBoard;
		Rules* myRules;

		int myCurrentPlayer;
		int mySpecialFigure;
		std::vector<Figure> mySetFigures[2];

		std::vector<Move> Moves(int player, std::vector<Figure>::iterator it_figure,  bool check=true);

		bool IsCheck(int player);

		std::vector<Figure>::iterator findFigure(int player, int figure);

		int getDirection(int dir) const;

	public:


		Model(Rules* _myRules);
		void Init(int mode);

		void Remove(Move move);
		bool CanMove(Move move, int* movetype=0);
		std::vector< Move > Moves(int player, Position pos1);
		std::vector< Move > Moves(int player);

		int GetBoard(int x, int y);
		int GetBoardSizeX() const;
		int GetBoardSizeY() const;
		std::string GetRulesName() const;
		GameStatus GetGameStatus(int player);
		const std::vector<Figure>& getSetFigures(int player) const;
		const FigureData& GetFigureData(int figure_id) const;
		int GetCurrentPlayer() const;

		void SetFigure(int player_id, const Figure& figure);
		void SetCurrentPlayer(int player_id);

		std::vector<Figure>::iterator findFigure(int player, Position find_pos);



};
#endif /* MODEL_H_ */
