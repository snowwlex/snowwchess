/*
 * game.h
 *
 *
 *      Author: snowwlex
 */

#ifndef GAME_H_
#define GAME_H_

#include "snowwchess.h"
#include "model/io/rules_io.h"
#include "player/player.h"
#include "view/gui_board_view.h"

//struct playerInfo {
//	std::string playerName;
//	int num1;
//	int num2;
//	playerInfo (std::string name, int n1, int n2) {
//		playerName = name;
//		num1 = n1;
//		num2 = n2;
//	}
//};


class Game : public Listener {
	public:
		Game();
		~Game();

	public: //initiating
		bool isReady();
		void prepare();

	public:
		void loadRules(const RulesIO& rulesIO);
		void setPlayer(int color, Player* player);
		void setBoardView(GuiBoardView* boardView);

	public:
		void start();
		void clear();

	public: //listener methods
		virtual void moveReady(const Move&);

	private:
		Model myModel;
		Rules myRules;
		GuiBoardView* myBoardView;
		Player* players[2];

	private:
		int curPlayer;
};



#endif /* GAME_H_ */
