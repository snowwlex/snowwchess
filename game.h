/*
 * game.h
 *
 *
 *      Author: snowwlex
 */

#ifndef GAME_H_
#define GAME_H_

struct playerInfo {
	std::string playerName;
	int num1;
	int num2;
	playerInfo (std::string name, int n1, int n2) {
		playerName = name;
		num1 = n1;
		num2 = n2;
	}
};

class Game {
	private:
	public:
		PlayerColor start(std::string file, int mode, playerInfo onePlayer, playerInfo twoPlayer);
};



#endif /* GAME_H_ */
