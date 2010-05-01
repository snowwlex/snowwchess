#ifndef GAME_H_
#define GAME_H_

#include "snowwchess.h"
#include "player/player.h"
#include "view/gui_board_view.h"
#include "view/gui_history_view.h"

class GuiHistoryView;

class Game : public Listener {
	public:
		Game();
		~Game();

	public: //initiating

	public:
		void setPlayer(int color, Player* player);
		void setBoardView(GuiBoardView* boardView);
		void setHistoryView(GuiHistoryView* historyView);
		void setModel(Model* model);

	public:
		void init();

	public: //listener methods
		void start();
		void stop();

	public:
		void setCurrentPlayer(int player) { myCurrentPlayer = player; }

	public:
		void changeModel(const Model& model);

	public: //listener methods
		virtual void moveReady(const Move&);

	private:
		Model* myModel;
		GuiBoardView* myBoardView;
		GuiHistoryView* myHistoryView;
		Player* myPlayers[2];

	private:
		int myCurrentPlayer;
};



#endif /* GAME_H_ */
