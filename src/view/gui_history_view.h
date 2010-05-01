#ifndef GUI_HISTORY_VIEW_H_
#define GUI_HISTORY_VIEW_H_

#include <QListWidget>

#include "../snowwchess.h"
#include "../model/model.h"
#include "../listener.h"
#include "../history.h"
#include "../game.h"
#include "gui_board_view.h"

class Game;

class GuiHistoryView : public QListWidget, public Listener, public Sender {
	Q_OBJECT

	public:
		GuiHistoryView(QWidget *parent = 0);
		~GuiHistoryView();

	public:
		void setModel(Model* model);
		void setBoardView(GuiBoardView* boardView) { myBoardView = boardView; }

	public slots:
		void makeUndo();


	public: //listener methods
		virtual void turnMaked(const Move& move);

	private: //sender methods
		void notifyStop() const;
		void notifyStart() const;

	public: //help method
		static QString moveToString(const Move& move, const Model& model );
	private: //fields
		Model* myModel;
		GuiBoardView* myBoardView;
		History myHistory;


};


#endif /* GUI_HISTORY_VIEW_H_ */
