#ifndef _PLAYER_H
#define _PLAYER_H


class Player {
	protected:
		
	public:
		//Player(PlayerColor color, WINDOW * user_w, WINDOW* info_w);
		virtual bool YourTurn() = 0;
};


class HumanPlayer : public Player {
	private:
		PlayerColor myColor;
		WINDOW *user_window;
		WINDOW *info_window;
		Model *model;
	public:
		HumanPlayer(PlayerColor color, WINDOW * user_w, WINDOW* info_w, Model* m);
		bool YourTurn();
};

#endif
