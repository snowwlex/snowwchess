#ifndef GAME_H_
#define GAME_H_

#include "../snowwchess.h"
#include "../player/player.h"
#include "../view/gui_board_view.h"
#include "../view/gui_history_view.h"

class GuiHistoryView; //becaouse history view calls Game methods (game as  Singleton)

class Game { //Singleton

public:
    static Game* instance();
    static void deleteInstance();
    ~Game();

public: //setters

    void setBoardView(GuiBoardView* boardView);
    void setHistoryView(GuiHistoryView* historyView);

public: //getters
    const Model& getModel() const;
    PlayerType getPlayerType(int color) const;
    int  getAILevel(int color) const ;

public: //public managment
    bool newGame(const std::string& fileName, const PlayerType* playerTypes, const int* aiLevels);
    bool loadGame(const std::string& fileName, const PlayerType* playerTypes, const int* aiLevels);
    bool saveGame(const std::string& fileName);
    void startGameProcess();
    void stopGameProcess();

public:
    void changeModel(const Model& model);
    void moveReady(int player, const SimpleMove& simpleMove);

private: //creating game helpers
    void setupModels();
    void setupListeners();
    void createPlayer(int color, PlayerType playerType, int aiLevel);

private: // inner managment
    void clear();
    void askForTurn();

private: // public preventions due to singleton pattern
    Game();

    Game(const Game&) {
    }

    void operator==(const Game&) {
    }

private: //singleton pattern
    static Game* myInstance;

private:

    bool gameInProcess;

    GuiBoardView* myBoardView;
    GuiHistoryView* myHistoryView;

    Model* myModel;
    History* myHistory;
    Rules* myRules;
    Player* myPlayers[2];

    PlayerType myPlayerTypes[2];
    int myAiLevels[2];



};



#endif /* GAME_H_ */
