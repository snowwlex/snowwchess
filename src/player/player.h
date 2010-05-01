#ifndef PLAYER_H_
#define PLAYER_H_

#include "../view/gui_board_view.h"
#include "../model/model.h"
#include "../listener.h"
#include "../sender.h"

class Player : public Listener, public Sender {

	public:
		virtual void makeTurn() = 0;

	public: //setters
		void setModel(Model* model) { myModel = model; }
		void setColor(int color)    { myColor = color; }
		void setBoardView(GuiBoardView* boardView) { myBoardView = boardView; }

	public: //sender methods
		void notifyMoveReady(const Move& move) const {
			for (LISTENERS::const_iterator itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
				(*itListener)->moveReady(move);
				qDebug() << ":Player:" << "calling of listener moveReady ";
			}
		}

	protected:
		int myColor;
		Model *myModel;
		GuiBoardView* myBoardView;

};






#endif /* PLAYER_H_ */
