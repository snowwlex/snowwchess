#ifndef ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_
#define ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_

#include <deque>

#include <QThread>
#include <QMutex>

#include "../snowwchess.h"
#include "alphabeta_search_ai_player.h"

class AlphaBetaParallelSearchAIPlayer;
typedef std::deque<Move> PULL_QUEUE;

struct Pull {
    AlphaBetaParallelSearchAIPlayer* player;
    PULL_QUEUE deque;
    Border curAlpha;
    Border curBeta;
    Move bestMove;
    bool stop;
    int threadsLeft;
    Pull() : curAlpha(0, -INF), curBeta(0, INF) {
    }
};


class ParallelSearchProcess : public QThread {
public:
    ParallelSearchProcess(Pull* pull);
public:
    virtual void run();

private:
    Pull* myPull;

};

class AlphaBetaParallelSearchAIPlayer : public AlphaBetaSearchAIPlayer {
public:
    AlphaBetaParallelSearchAIPlayer(int depth = 0, int addDepth = 0);
    ~AlphaBetaParallelSearchAIPlayer();
public:
    virtual void getMove();

public: //listener
    void stopGetMove();

private:

    void allDone();

private:
    Pull myPull;
    QMutex* myMutex;
    ParallelSearchProcess* process[2];

    friend class ParallelSearchProcess;

};




#endif /* ALPHABETA_PARALLEL_SEARCH_AI_PLAYER_H_ */
