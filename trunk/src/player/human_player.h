/*
 * human_player.h
 *
 *  Created on: 24.04.2010
 *      Author: snowwlex
 */

#ifndef HUMAN_PLAYER_H_
#define HUMAN_PLAYER_H_

#include "player.h"

class HumanPlayer : public Player {

	public:
		HumanPlayer();

	public:
		virtual void makeTurn();

	public: //listener methods
		virtual void pushedCell(const Position& pos);

	private: //helper methods
		void catchStartCell(const Position& pos);
		void catchFinishCell(const Position& pos);

	public:
		bool iAmMoving;
		bool catchDestination;
		Move myMove;

};


#endif /* HUMAN_PLAYER_H_ */
