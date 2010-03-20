/*
 * ai.h
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#ifndef AI_H_
#define AI_H_

class AIPlayer : public Player {
	protected:
		int myColor;
		int myTurnsCounter;
		int myDepth;
		int myCounter;
		Model *myModel;
		BoardCLIView *myBoardView;
		CLIView *myUserView;

		int sefMaterial(const Model& model, int player) const;
	public:
		AIPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView, int depth);
};

class FullSearchAIPlayer : public AIPlayer {
	private:
		int miniMaxSearch(Move& returnMove, int curDepth, int curPlayer, const Model& model);
	public:
		FullSearchAIPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView, int depth);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};

class AlphaBetaSearchAIPlayer : public AIPlayer {
	private:
		enum Infinity { INF=1 };
		struct Border {
			int myIsInfinity;
			int myValue;
			Border(int value, int isInfinity=0) : myIsInfinity(isInfinity), myValue(value) { }
			Border operator-() const {
				return Border(-myValue, -myIsInfinity);
			}
			bool operator<(const Border& border) const {
				if (myIsInfinity > border.myIsInfinity) return false;
				if (myIsInfinity < border.myIsInfinity) return true;
				if (myIsInfinity == 0 && myValue < border.myValue) return true;
				return false;
			}
			bool operator>(const Border& border) const {
				if (myIsInfinity > border.myIsInfinity) return true;
				if (myIsInfinity < border.myIsInfinity) return false;
				if (myIsInfinity == 0 && myValue > border.myValue) return true;
				return false;
			}
			bool operator==(const Border& border) const {
				return myIsInfinity==border.myIsInfinity && myValue==border.myValue;
			}
		};

		int alphaBetaNegaMaxSearch(Move& returnMove, Border alpha, Border beta, int curPlayer, int curDepth, const Model& model);

	public:
		AlphaBetaSearchAIPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView, int depth);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};
#endif /* AI_H_ */
