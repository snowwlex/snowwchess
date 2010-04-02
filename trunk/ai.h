/*
 * ai.h
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#ifndef AI_H_
#define AI_H_

#define DEBUG_DISPLAY_SEF  sprintf(buffer,"sef = %d\n",sefMaterial(model, curPlayer)); debugView->render(buffer);
#define DEBUG_DISPLAY_BEFORE  sprintf(buffer,"[%d] BEFORE: %d:%d\n",curDepth, alpha.myIsInfinity==0?alpha.myValue:999, beta.myIsInfinity==0?beta.myValue:999);debugView->render(buffer);
#define DEBUG_DISPLAY_SCORE  sprintf(buffer,"[AB] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,tmpScore); debugView->render(buffer);
#define DEBUG_DISPLAY_MOVE  sprintf(buffer,"[move] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' ]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter); debugView->render(buffer);
#define DEBUG_DISPLAY_AFTER sprintf(buffer,"[%d] AFTER: score=%d, %d:%d\n",curDepth, score.myIsInfinity==0?score.myValue:999,alpha.myIsInfinity==0?alpha.myValue:999, beta.myIsInfinity==0?beta.myValue:999);debugView->render(buffer);
#define DEBUG_DISPLAY_RETURN sprintf(buffer,"[%d]%s RETURN: %d\n",curDepth, pruning==true?"pruninged":"", score.myIsInfinity==0?score.myValue:999); debugView->render(buffer);
#define DEBUG_DISPLAY(something) sprintf(buffer,something); debugView->render(buffer);

#define DEBUG_LOG_SEF fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"sef = %d\n",sefMaterial(model, curPlayer)); fputs(buffer,statfile);
#define DEBUG_LOG_BEFORE fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"[%d] BEFORE: %d:%d\n",curDepth, alpha.myIsInfinity==0?alpha.myValue:999, beta.myIsInfinity==0?beta.myValue:999); fputs(buffer,statfile);
#define DEBUG_LOG_SCORE fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"[++] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' SCORE %d]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter,tmpScore); fputs(buffer,statfile);
#define DEBUG_LOG_MOVE fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile);  sprintf(buffer,"[move] %c%c-%c%c, eff=%d, type=%d,pl=%d, '%c' ]\n",itMove->pos1.myX+'a',myModel->getBoardSizeY() - itMove->pos1.myY + '0',itMove->pos2.myX+'a',myModel->getBoardSizeY() - itMove->pos2.myY + '0',itMove->effect,itMove->type,itMove->player,myModel->getFigureData(itMove->figureId).letter); debugView->render(buffer);  fputs(buffer,statfile);
#define DEBUG_LOG_AFTER fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile); sprintf(buffer,"[%d] AFTER: score=%d, %d:%d\n",curDepth, score.myIsInfinity==0?score.myValue:999,alpha.myIsInfinity==0?alpha.myValue:999, beta.myIsInfinity==0?beta.myValue:999);fputs(buffer,statfile);
#define DEBUG_LOG_RETURN fputs(std::string(3*(myDepth-curDepth),' ').c_str(),statfile);sprintf(buffer,"[%d]%s RETURN: %d\n",curDepth, pruning==true?"pruninged":"", score.myIsInfinity==0?score.myValue:999); fputs(buffer,statfile);

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

class AIPlayer : public Player {
	protected:
		int myColor;
		int myTurnsCounter;
		int myDepth;
		int myCounter;
		int myQCounter;
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
		int alphaBetaNegaMaxSearch(Move& returnMove, Border alpha, Border beta, int curPlayer, int curDepth, const Model& model);
		int quiesSearch(Move& returnMove, Border alpha, Border beta, int curPlayer, int curDepth , const Model& model);
	public:
		bool operator()(const Move& move1,const Move& move2);
		AlphaBetaSearchAIPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView, int depth);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};

class AlphaBetaParallelSearchAIPlayer;

typedef std::deque<Move> PULL_QUEUE;
struct Pull {
	AlphaBetaParallelSearchAIPlayer* player;
	PULL_QUEUE deque;
	Border curAlpha;
	Border curBeta;
	Move bestMove;
	Pull() :  curAlpha(0,-INF), curBeta(0,INF) { }
};

class AlphaBetaParallelSearchAIPlayer : public AIPlayer {
	private:

		Pull myPull;

		int alphaBetaNegaMaxSearch(Border alpha, Border beta, int curPlayer, int curDepth, const Model& model);
		int quiesSearch(Border alpha, Border beta, int curPlayer, int curDepth , const Model& model);
		friend void* parallelSearch(void*);
	public:
		bool operator()(const Move& move1,const Move& move2);
		AlphaBetaParallelSearchAIPlayer(int color, Model* m, BoardCLIView *boardView, CLIView * userView, int depth);
		virtual PlayerCommand makeTurn(Move& move, GameMessage message = NONE);
};

#endif /* AI_H_ */
