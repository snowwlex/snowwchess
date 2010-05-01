/*
 * ai.h
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#ifndef AI_H_
#define AI_H_

#include "../snowwchess.h"
#include "player.h"

class AIPlayer : public Player {
	public:
		virtual void makeTurn() = 0;

	public: //setters
		void setDepth(int depth);
		void setAddDepth(int addDepth);
		void setABEqualPruning(bool b) { abEqualPruning = b; }

	public: //help methods
		bool operator()(const Move& move1,const Move& move2); //for moves sort

	protected:
		AIPlayer() { myTurnsCounter = 0; abEqualPruning = false; }

	protected:
		int sefMaterial(const Model& model, int player) const;

	protected:
		int myDepth;
		int myAddDepth;
		bool abEqualPruning;

	protected: //for debug
		int myTurnsCounter;
		int myCounter;
		int myQCounter;


};






#endif /* AI_H_ */
