/*
 * model.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_H_
#define MODEL_H_

//ENUMS
enum GameStatus {USUAL, CHECK, MATE, STALEMATE };

//CLASSES
class Board {
	private:
		const int myBufferSize;
		int** myBoardArray;
	public:
		int mySizeX, mySizeY;
		Board(int buffer=4); // 4 - размер буферной зоны
		~Board();
		void Init(int sizex, int sizey);
		void Set( std::vector<Figure> figure_set);
		int& operator() (int x,int y);
};
class Model {
	private:

		// для unmove:
		Figure erased, moved;

		Board myBoard;
		Rules* myRules;
		std::vector<Figure> mySetFigures[2];
		std::vector<Figure>::iterator findFigure(int player, Position find_pos);
		std::vector<Figure>::iterator findFigure(int player, int figure);
		std::vector<Move> Moves(int player, std::vector<Figure>::iterator it_figure,  bool check=true);
		void Unmove(Move move);
		bool IsCheck(int player);
	public:
		Model(Rules* _myRules);
		void InitBoard();
		int GetBoardSizeX() const;
		int GetBoardSizeY() const;
		GameStatus GetGameStatus(int player);
		void Remove(Move move);
		bool CanMove(Move move);
		std::vector< Move > Moves(int player, Position pos1);
		std::vector< Move > Moves(int player);
		void SetInitFigures(int player);

		const std::vector<Figure>& getSetFigures(int player) const;
		const FigureData& GetFigureData(int figure_id) const;

		void SetFigures(int player_id, const std::vector<Figure>& setfigures);

};
#endif /* MODEL_H_ */
