/*
 * model.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_H_
#define MODEL_H_

//CLASSES
class Board {
	private:
		const int myBufferSize;
		int** myBoardArray;
		int mySizeX, mySizeY;
	public:
		Board(int buffer=4); // 4 - размер буферной зоны
		~Board();
		void Init(int sizex, int sizey);
		void Set(const std::vector<Figure>& figure_set);
		int& operator() (int x,int y);
};
class Rules;
class Model {
	private:

		// для unmove:
		Figure erased, moved;

		Board myBoard;
		Rules* myRules;

		int myCurrentPlayer;
		int mySpecialFigure;
		std::vector<Figure> mySetFigures[2];

		std::vector<Move> Moves(int player, std::vector<Figure>::iterator it_figure,  bool check=true);
		void Unmove(Move move);
		bool IsCheck(int player);

		std::vector<Figure>::iterator findFigure(int player, Position find_pos);
		std::vector<Figure>::iterator findFigure(int player, int figure);

	public:


		Model(Rules* _myRules);
		void Init(int mode);

		void Remove(Move move);
		bool CanMove(Move move);
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

		void SetFigures(int player_id, const std::vector<Figure>& setfigures);
		void SetCurrentPlayer(int player_id);





};
#endif /* MODEL_H_ */
