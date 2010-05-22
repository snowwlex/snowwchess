#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "alphabeta_parallel_search_ai_player.h"
#include "../model/game.h"

AlphaBetaParallelSearchAIPlayer::AlphaBetaParallelSearchAIPlayer(int depth, int addDepth)
: AlphaBetaSearchAIPlayer(depth, addDepth) {
    myMutex = new QMutex;
    process[0] = new ParallelSearchProcess(&myPull);
    process[1] = new ParallelSearchProcess(&myPull);
}

AlphaBetaParallelSearchAIPlayer::~AlphaBetaParallelSearchAIPlayer() {
    delete myMutex;
    delete process[0];
    delete process[1];
}

void AlphaBetaParallelSearchAIPlayer::stopGetMove() {
    myMutex->lock();
    myPull.stop = true;
    myMutex->unlock();
}

void AlphaBetaParallelSearchAIPlayer::getMove() {

    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove()";
    ++myTurnsCounter;
    myCounter = 0;
    myQCounter = 0;
    srand(time(0));

    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove()" << " getting all moves";
    MOVES moves = myModel->allMoves(myColor, MOVE | CAPTURE);
    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove()" << " sorting moves" << moves.size();
    std::sort(moves.begin(), moves.end(), sortMVV_LVA(myModel) );
    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove()" << " sorting moves ended";


    myMutex->lock();
    myPull = Pull();
    myPull.stop = false;
    myPull.deque.insert(myPull.deque.begin(), moves.begin(), moves.end());
    myPull.player = this;
    myPull.threadsLeft = 2;
    myMutex->unlock();

    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove()" << " filling pull ended";
   
    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove() starting process1";
    process[0]->start();
    qDebug() << "AlphaBetaParallelSearchAIPlayer::getMove() starting process2";
    process[1]->start();
}

void AlphaBetaParallelSearchAIPlayer::allDone() {

    Move bestMove = myPull.bestMove;

    qDebug() << ":AlphaBetaSearchAIPlayer:"
            << "My AB bestMove #" << myTurnsCounter
            << "[" << bestMove.pos1.myX + 'a' << myModel->getBoardSizeY() - bestMove.pos1.myY + '0'
            << "-" << bestMove.pos2.myX + 'a' << myModel->getBoardSizeY() - bestMove.pos2.myY + '0'
            << "] (" << bestMove.type;
    qDebug() << ":AlphaBetaSearchAIPlayer:"
            << "[AB] My counter= " << myCounter
            << "qcounter=" << myQCounter
            << "myDepth = " << myDepth
            << "turns =" << myTurnsCounter;
    //qDebug() << ":AlphaBetaParallelSearchAIPlayer:" << "Time for searching:" << difftime(end,start);
    if (myPull.stop == false) {
        Game::instance()->moveReady(myColor, bestMove);
    }



}

ParallelSearchProcess::ParallelSearchProcess(Pull* pull) {
    myPull = pull;
}

void ParallelSearchProcess::run() {

    qDebug() << "ParallelSearchProcess::run()  started";

    bool stop;
    Border alpha(0), beta(0);
    Move move;

    stop = false;


    while (stop == false) {

        myPull->player->myMutex->lock();
        if (myPull->deque.empty() == false) {
            move = myPull->deque.front();
            myPull->deque.pop_front();
            alpha = myPull->curAlpha;
            beta = myPull->curBeta;
        } else {
            stop = true;
        }
        myPull->player->myMutex->unlock();

        if (stop == false) {
            Model curModel = *myPull->player->myModel;
            curModel.makeMove(move);
            int score = -myPull->player->alphaBetaNegaMaxSearch(-beta, -alpha, 1 - myPull->player->myColor, myPull->player->myDepth - 1, curModel);

            qDebug() << ":AlphaBetaParallelSearchAIPlayer: "
                    << move.pos1.myX + 'a' << myPull->player->myModel->getBoardSizeY() - move.pos1.myY + '0'
                    << move.pos2.myX + 'a' << myPull->player->myModel->getBoardSizeY() - move.pos2.myY + '0'
                    << move.effect
                    << move.type
                    << move.player
                    << myPull->player->myModel->getFigureData(move.figureId).letter
                    << score
                    << this->currentThreadId();


            myPull->player->myMutex->lock();
            if (myPull->curAlpha < score) {
                myPull->bestMove = move;
                myPull->curAlpha = score;
            } else if (myPull->curAlpha == score && rand() % 2) {
                myPull->bestMove = move;
            }
            myPull->player->myMutex->unlock();
        }

    }

    qDebug() << ":AlphaBetaParallelSearchAIPlayer: " << "thread ends counting";


    myPull->player->myMutex->lock();
    myPull->threadsLeft -= 1;
    if (myPull->threadsLeft == 0) {
        qDebug() << ":AlphaBetaParallelSearchAIPlayer: " << "call allDone() ";
        myPull->player->allDone();
    }
    myPull->player->myMutex->unlock();

}

