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
		//Player(PlayerColor color, WINDOW * user_w, WINDOW* info_w);
		virtual PlayerCommand YourTurn(Move& move, GameMessage message = NONE) = 0;
};


class HumanPlayer : public Player {
	private:
		int myColor;
		Model *model;
		CLIView *myBoardView;
		CLIView *myUserView;
	public:
		HumanPlayer(int _color, Model* m, CLIView *board_view, CLIView * user_view);
		virtual PlayerCommand YourTurn(Move& move, GameMessage message = NONE);
};

#endif /* PLAYER_H_ */
