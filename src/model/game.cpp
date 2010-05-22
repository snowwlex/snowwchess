#include <QMessageBox>
#include <cassert>


#include "../model/io/rules_io.h"
#include "../model/io/model_io.h"
#include "../model/model.h"
#include "../model/rules.h"
#include "../player/human_player.h"
#include "../player/alphabeta_parallel_search_ai_player.h"
#include "../player/alphabeta_search_ai_player.h"

#include "game.h"



Game* Game::myInstance = 0;

Game* Game::instance() {
    if (myInstance == 0) {
        myInstance = new Game;
    }
    return myInstance;
}

void Game::deleteInstance() {
    if (myInstance != 0) {
        delete myInstance;
    }
}

Game::Game() {

    gameInProcess = false;

    //objects under responsibility of class Game
    myPlayers[WHITE] = myPlayers[BLACK] = 0;
    myModel = 0;
    myRules = 0;
    myHistory = 0;

    //settable views
    myBoardView = 0;
    myHistoryView = 0;
}

Game::~Game() {
    clear();
}

void Game::clear() {
    stopGameProcess();
    delete myPlayers[WHITE];
    delete myPlayers[BLACK];
    delete myHistory;
    delete myModel;
    delete myRules;
}

void Game::stopGameProcess() {
    if (gameInProcess == false) {
        return;
    }
    myBoardView->setActivePlayer(0);
    myPlayers[WHITE]->stopGetMove();
    myPlayers[BLACK]->stopGetMove();
    gameInProcess = false;
}

const Model& Game::getModel() const {
    assert(myModel != 0);
    return *myModel;
}

void Game::setBoardView(GuiBoardView* boardView) {
    myBoardView = boardView;
}

void Game::setHistoryView(GuiHistoryView* historyView) {
    myHistoryView = historyView;
}

void Game::createPlayer(int color, PlayerType playerType, int aiLevel) {
    switch (playerType) {
        case HUMAN:
            myPlayers[color] = new HumanPlayer();
            break;
        case AI:
            AIPlayer* player = 0;
            switch (aiLevel) {
                case 0:
                    player = new AlphaBetaParallelSearchAIPlayer(2, 0);
                    break;
                case 1:
                    player = new AlphaBetaParallelSearchAIPlayer(3, 10);
                    break;
                case 2:
                    player = new AlphaBetaParallelSearchAIPlayer(4, 10);
                    break;
                case 3:
                    player = new AlphaBetaParallelSearchAIPlayer(5, 3);
                    break;
            }
            assert(player != 0);
            player->setABEqualPruning(true);
            myPlayers[color] = player;
            break;
    }

    myPlayerTypes[color] = playerType;
    myAiLevels[color] = aiLevel;


    myPlayers[color]->setColor(color);


}

void Game::changeModel(const Model& model) {
    qDebug() << ":Game:" << "changing model";
    if (myModel != 0) {
        stopGameProcess();
        *myModel = model;
        myModel->addSubscriber(myBoardView);
        myBoardView->updateIt();
        startGameProcess();
    }
}

void Game::moveReady(int player, const SimpleMove& simpleMove) {

    qDebug() << ":Game: " << "got  move: " << simpleMove.pos1.myX << simpleMove.pos1.myY
            << simpleMove.pos2.myX << simpleMove.pos2.myY;

    if (player != myModel->getCurrentPlayer()) {
        return;
    }

    if (myModel->canMove(simpleMove) == false) {
        return;
    }

    myModel->makeMove(simpleMove);

    Move moveHistory;
    bool ok = myModel->getLastMove(moveHistory);
    assert(ok == true);
    myHistory->addRecord(moveHistory, *myModel);

    this->myBoardView->updateIt();

    qDebug() << ":Game: " << " move made, ask for next turn ";

    askForTurn();

}

void Game::askForTurn() {

    assert(myModel != 0);
    assert(myBoardView != 0);
    assert(myPlayers[WHITE] != 0);
    assert(myPlayers[BLACK] != 0);

    int currentPlayer = myModel->getCurrentPlayer();

    qDebug() << "Game::askForTurn() checking game status";
    GameStatus status = myModel->checkGameStatus(currentPlayer);
    if (status == MATE) {
        QString playerName = currentPlayer == WHITE ? "white" : "black";
        QMessageBox("game message", QString("player ") + playerName + " has got the mate!",
                QMessageBox::Information, QMessageBox::Ok, 0, 0).exec();
        stopGameProcess();
    } else if (status == STALEMATE) {
        QMessageBox("game message", "It's the stalemate!",
                QMessageBox::Information, QMessageBox::Ok, 0, 0).exec();
        stopGameProcess();
    } else {
        myBoardView->setActivePlayer(myPlayers[currentPlayer]);

        qDebug() << "Game::askForTurn() asking player for a turn";
        myPlayers[currentPlayer]->getMove();
    }


}

void Game::setupModels() {

    assert(myModel != 0);

    assert(myBoardView != 0);
    assert(myHistoryView != 0);

    assert(myPlayers[WHITE] != 0);
    assert(myPlayers[BLACK] != 0);

    myBoardView->setModel(myModel);
    myHistoryView->setHistory(myHistory);
    for (int i = 0; i < 2; ++i) {
        myPlayers[i]->setModel(myModel);
    }
}

void Game::setupListeners() {

    assert(myModel != 0);
    assert(myHistory != 0);

    assert(myBoardView != 0);
    assert(myHistoryView != 0);

    myModel->addSubscriber(myBoardView);
    myHistory->addSubscriber(myHistoryView);

}

void Game::startGameProcess() {
    gameInProcess = true;
    askForTurn();
}

bool Game::loadGame(const std::string& fileName, const PlayerType* playerTypes, const int* aiLevels) {

    stopGameProcess();
    clear();


    RulesIO rulesIO;
    ModelIO modelIO;
    modelIO.load(fileName);
    bool ok = rulesIO.load("rules/" + modelIO.getStorage().rulesName + ".xml");
    if (ok == false) {
        return false;
    }

    myRules = new Rules;
    myModel = new Model;

    rulesIO.updateRules(*myRules);
    myModel->setRules(myRules);
    modelIO.updateModel(*myModel);
    myModel->init(false);

    myHistory = new History(*myModel); //consructed with Model (and after Model's initiate),
    //because History at first take the inital snapshot of model
    createPlayer(WHITE, playerTypes[WHITE], aiLevels[WHITE]);
    createPlayer(BLACK, playerTypes[BLACK], aiLevels[BLACK]);
    setupModels();
    setupListeners();
    startGameProcess();

    return true;
}

bool Game::newGame(const std::string& fileName, const PlayerType* playerTypes, const int* aiLevels) {

    stopGameProcess();
    clear();


    RulesIO rulesIO;
    bool ok = rulesIO.load(fileName);
    if (ok == false) {
        return false;
    }

    myRules = new Rules;
    myModel = new Model;

    rulesIO.updateRules(*myRules);
    myModel->setRules(myRules);
    myModel->init(true);

    //consructed with Model (and after Model's initiate),
    //because History at first take the inital snapshot of model
    myHistory = new History(*myModel);

    createPlayer(WHITE, playerTypes[WHITE], aiLevels[WHITE]);
    createPlayer(BLACK, playerTypes[BLACK], aiLevels[BLACK]);

    setupModels();
    setupListeners();
    startGameProcess();

    return true;

}

bool Game::saveGame(const std::string& fileName) {
    if (myModel == 0) {
        return false;
    }
    ModelIO modelIO;
    modelIO.save(fileName, *myModel);
    return true;
}

PlayerType Game::getPlayerType(int color) const {
    return myPlayerTypes[color];
}

int Game::getAILevel(int color) const {
    return myAiLevels[color];
}