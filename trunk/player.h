/*
 * player.h
 *
 *
 *      Author: snowwlex
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "snowwchess.h"
#include "guiboardview.h"
#include "model.h"
#include "listener.h"
#include "sender.h"

class Player : public Listener, public Sender {

	public:
		virtual void makeTurn() = 0;

	public: //setters
		void setModel(Model* model) { myModel = model; }
		void setColor(int color)    { myColor = color; }
		void setBoardView(GuiBoardView* boardView) { myBoardView = boardView; }

	public: //sender methods
		void notifyMoveReady(const Move& move) const;

	protected:
		int myColor;
		Model *myModel;
		GuiBoardView* myBoardView;

};


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




#endif /* PLAYER_H_ */
