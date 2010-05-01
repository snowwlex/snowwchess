#include <cassert>

#include "gui_history_view.h"


GuiHistoryView::GuiHistoryView(QWidget *parent)
    : QListWidget(parent)
{

	myModel = 0;

}

void GuiHistoryView::setModel(Model* model) {
	myModel = model;
}

GuiHistoryView::~GuiHistoryView() {

}

void GuiHistoryView::makeUndo() {




	qDebug() << ":GuiHistoryView:" << "making model undo";
	if (myHistory.getMoves().size() > 2) {
		notifyStop();
		*myModel = myHistory.makeUndo(2);
		notifyStart();

	}
	qDebug() << ":GuiHistoryView:" << "made model undo";


	myBoardView->updateIt();
}

void GuiHistoryView::notifyStop() const {
	for (LISTENERS::const_iterator itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
		(*itListener)->stop();
		qDebug() << ":GuiHistoryView:" << "calling of listener for notifyStop";
	}
}

void GuiHistoryView::notifyStart() const {
	for (LISTENERS::const_iterator itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
		(*itListener)->start();
		qDebug() << ":GuiHistoryView:" << "calling of listener for notifyStart";
	}
}

void GuiHistoryView::turnMaked(const Move& move) {
	assert(myModel != 0);
	QString string = moveToString(move, *myModel);
	this->addItem(string);
	myHistory.addRecord(move, *myModel);
}


QString GuiHistoryView::moveToString(const Move& move, const Model& model ) {

	int sizeY = model.getBoardSizeY();

	char letter1 = move.pos1.myX + 'a';
	char letter2 = move.pos2.myX + 'a';
	char digit1  = sizeY - move.pos1.myY + '0';
	char digit2  = sizeY - move.pos2.myY + '0';

	char moveString[] = { letter1, digit1, '-', letter2, digit2, '\0' };

	QString name = model.getFigureData(move.figureId).name.c_str();
	QString player = move.player == WHITE ? "W" : "B";
	QString type;
	switch (move.type) {
		case CAPTURE:
		case INPASSING:
			type = "capture";
			break;
		case MOVE:
			type = "move";
			break;
	}
	QString effect;
	switch (move.effect) {
		case CASTLE:
			effect = "castling";
	}

	QString result;
	result += player + "  ";
	result += moveString;
	result += "  " + name + "  ";
    result += type + "  " + effect;
	return result;

}
//
//void GuiBoardView::notifyClickedCell(const Position& pos) const {
//	for (LISTENERS::const_iterator itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
//		(*itListener)->pushedCell(pos);
//		qDebug() << ":GuiBoardView:" << "calling of listener clickedCell";
//	}
//}
//
//void GuiBoardView::turnMaked(const Move&) {
//	redrawBoardFigures = true;
//	this->update();
//}
