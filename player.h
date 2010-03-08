/*
 * player.h
 *
 *
 *      Author: snowwlex
 */

#ifndef PLAYER_H_
#define PLAYER_H_


class Player {
	protected:

	public:
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE) = 0;
};


class HumanPlayer : public Player {
	private:
		int myColor;
		Position myCursorPos;
		Model *myModel;
		BoardCLIView *myBoardView;
		CLIView *myUserView;
	public:
		HumanPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};

class HumanAltPlayer : public Player {
	private:
		int myColor;
		Model *myModel;
		BoardCLIView *myBoardView;
		CLIView *myUserView;
	public:
		HumanAltPlayer(int color, Model* model, BoardCLIView *boardView, CLIView * userView);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};


#endif /* PLAYER_H_ */
