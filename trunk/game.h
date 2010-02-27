/*
 * game.h
 *
 *
 *      Author: snowwlex
 */

#ifndef GAME_H_
#define GAME_H_
enum PlayerCommand {NOTHING, TURN, SAVE, EXIT };
enum GameMessage {NONE, WRONG_MOVE, CHECK};

class Game {
	private:
	public:
		void Start();
};

#endif /* GAME_H_ */
