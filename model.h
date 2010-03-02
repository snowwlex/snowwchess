/*
 * model.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_H_
#define MODEL_H_

#define BUFFER_SIZE 2
#define BOARD_SIZE 8

//ENUMS
enum GameStatus {USUAL, CHECK, MATE, STALEMATE };

//CLASSES
class Board {
	private:
		// 8x8 - поле,
	    //+2 клетки с каждой стороны на буферную зону
		int myBoardArray[BOARD_SIZE+2*BUFFER_SIZE][BOARD_SIZE+2*BUFFER_SIZE];
	public:
		Board();
		void Set( std::vector<Figure> figure_set);
		int& operator() (int x,int y);
};
class Model {
	friend class Rules;
	private:

		// для unmove:
		Figure erased, moved;

		Board myBoard;
		Rules *myRules;
		std::vector<Figure> mySetFigures[2];
		std::vector<Figure>::iterator findFigure(int player, Position find_pos);
		std::vector<Figure>::iterator findFigure(int player, int figure);
		std::vector<Move> Moves(int player, std::vector<Figure>::iterator it_figure,  bool check=true);
		void Unmove(Move move);
		bool IsCheck(int player);
	public:
		Model(Rules* _myRules);
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
