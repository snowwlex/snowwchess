/*
 * game.h
 *
 *
 *      Author: snowwlex
 */

#ifndef GAME_H_
#define GAME_H_

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

#include "snowwchess.h"
#include "model.h"
#include "rules.h"
#include "rules_io.h"
#include "guiboardview.h"
#include "player.h"

class Game {
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

	private:
		Model myModel;
		Rules myRules;
		GuiBoardView* myBoardView;
		Player* players[2];
};



#endif /* GAME_H_ */
