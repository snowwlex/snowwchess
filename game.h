/*
 * game.h
 *
 *
 *      Author: snowwlex
 */

#ifndef GAME_H_
#define GAME_H_
enum PlayerCommand {NOTHING, TURN, SAVE, EXIT };
enum GameMessage {NONE, WRONG_MOVE, GOT_CHECK, SAVED};

class Game {
	private:
	public:
		void Start(std::string file, int mode);
};

#endif /* GAME_H_ */
