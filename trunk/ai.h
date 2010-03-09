/*
 * ai.h
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#ifndef AI_H_
#define AI_H_

class AIPlayer : public Player {
	private:
		int myColor;
		int myDepth;
		int myCounter;
		Model *myModel;
		BoardCLIView *myBoardView;
		CLIView *myUserView;

		Move search();
		int searchRecurs(Model m, Move move, int curDepth, int max);
		int sef(Model *m) const;
	public:
		AIPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};

#endif /* AI_H_ */
