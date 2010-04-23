/*
 * guiboardview.cpp
 *
 *  Created on: 10.04.2010
 *      Author: snowwlex
 */

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include <algorithm>

#include "guiboardview.h"
#include "listener.h"

GuiBoardView::GuiBoardView(QWidget *parent)
    : QWidget(parent)
{
	myModel = 0;
	redrawBoardCells = true;
	redrawBoardFigures = true;
	redrawBoardHighlightCells = true;
}

void GuiBoardView::countSizes() {
	QSize tmpBoardSize (myModel->getBoardSizeX(), myModel->getBoardSizeY() );
	QSize tmpScreenSize = this->size();

	if (tmpBoardSize != boardSize || tmpScreenSize != screenSize) {

		boardSize = tmpBoardSize;
		screenSize = tmpScreenSize;

		int leastSize = std::min( screenSize.width() , screenSize.height() );
		int cellWidth = leastSize / boardSize.width();
		int cellHeight = leastSize / boardSize.height();
		cellSize = QSize(cellWidth, cellHeight);

		redrawBoardCells = true;
		redrawBoardFigures = true;
		redrawBoardHighlightCells = true;

		qDebug() << ":GuiBoardView:" << "recounted size:: "
		         << boardSize << screenSize << cellSize;

	}
}

void GuiBoardView::prepareBoardCells() {

	if (redrawBoardCells == false) {
		return;
	}

	redrawBoardCells = false;

	QColor  whiteCellColor(255,247,153),
			blackCellColor(140,98,57);


	myBoardCells = QPixmap(screenSize);
	QPainter painter(&myBoardCells);

	// black color background
	myBoardCells.fill(Qt::black);

	//board drawing
	painter.setPen(Qt::NoPen);
	painter.setBrush(whiteCellColor);
	painter.drawRect( QRect( QPoint(0,0), QSize(cellSize.width()*boardSize.width(),cellSize.height()*boardSize.height() )) );
	painter.setBrush(blackCellColor);

	for (int i = 0; i != boardSize.height(); ++i) {
		for (int j = 0; j != boardSize.width(); ++j) {
			if ( (i+j)%2 == 0) {
				painter.drawRect( QRect( QPoint(cellSize.width()*j,cellSize.height()*i), cellSize ) );
			}
		}
	}


}



void GuiBoardView::prepareFigures() {

	if (redrawBoardFigures == false) {
			return;
	}


	redrawBoardFigures = false;

	myBoardFigures = QPixmap(screenSize);
	myBoardFigures.fill(Qt::transparent);


	QPainter painter(&myBoardFigures);
	//painter.setRenderHint(QPainter::Antialiasing, true);

	FIGURES::const_iterator itFigure;
	for (int i=0; i<2; ++i) {
		for ( itFigure=myModel->getSetFigures(i).begin(); itFigure != myModel->getSetFigures(i).end(); ++itFigure ) {
			if (itFigure->captured == false) {
//				qDebug() << ":GuiBoardView:" << (i==0? "white":"black")
//						 << "id: " << itFigure->id
//						 << "position: x: " << itFigure->position.myX << " y: " << itFigure->position.myY
//						 << myModel->getFigureData(itFigure->id).picture[WHITE].c_str()
//						 << myModel->getFigureData(itFigure->id).picture[BLACK].c_str();

				QPixmap picture(myModel->getFigureData(itFigure->id).picture[i].c_str());
				QPoint point(cellSize.width() * itFigure->position.myX, cellSize.height() * itFigure->position.myY);

				picture = picture.scaledToWidth(cellSize.width());
				painter.drawPixmap(point, picture);
			}
		}
	}

}


void GuiBoardView::highlight(const HIGHLIGHT_CELLS& highlightCells) {
	myHighlightCells = highlightCells;
	redrawBoardHighlightCells = true;
	this->update();
}
void GuiBoardView::clearHightlights() {
	myHighlightCells.clear();
	redrawBoardHighlightCells = true;
	this->update();
}

void GuiBoardView::prepareHighlightCells() {

	if (redrawBoardHighlightCells == false) {
		return;
	}



	redrawBoardHighlightCells = false;

	myBoardHighlightCells = QPixmap(screenSize);
	myBoardHighlightCells.fill(Qt::transparent);
	QPainter painter(&myBoardHighlightCells);
	painter.setPen(Qt::NoPen);


	Position pos;
	QColor color;
	for (HIGHLIGHT_CELLS::iterator itHighlight = myHighlightCells.begin(); itHighlight != myHighlightCells.end(); ++itHighlight ) {
		 pos = itHighlight->first;
		 color =  itHighlight->second;
		 qDebug() << ":GuiBoardView:" << pos.myX << pos.myY << color;
		 painter.setBrush(color);
		 painter.drawRect( QRect(  QPoint(cellSize.width()*pos.myX,cellSize.height()*pos.myY), cellSize ) );
	}



}


void GuiBoardView::setModel(Model* model) {
	myModel = model;
}
void GuiBoardView::paintEvent(QPaintEvent*) {
	qDebug() << ":GuiBoardView:" << "paint event rendering";

	if (myModel == 0) {
		qDebug() << ":GuiBoardView:" << "model is not setted";
		return;
	}

	countSizes();
	prepareBoardCells();
	prepareFigures();
	prepareHighlightCells();

	QPainter painter(this);
	QPoint leftTopEdge(0,0);
	//painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawPixmap( leftTopEdge, myBoardCells);
	painter.drawPixmap( leftTopEdge, myBoardHighlightCells);
	painter.drawPixmap( leftTopEdge, myBoardFigures);

	qDebug() << ":GuiBoardView:" << "rendering of board view is finished";

}

void GuiBoardView::mousePressEvent(QMouseEvent *pe) {
	if ( pe->buttons() & Qt::LeftButton) {

		if (myModel != 0) {
			countSizes();
			Position pos;
			pos.myX = pe->x() / cellSize.width();
			pos.myY = pe->y() / cellSize.height();
			qDebug() << ":GuiBoardView:" << "CELL IS " << pos.myX << pos.myY;
			notifyClickedCell(pos);
		}
	}
}
GuiBoardView::~GuiBoardView() {
}

void GuiBoardView::notifyClickedCell(const Position& pos) const {
	for (LISTENERS::const_iterator itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
		(*itListener)->pushedCell(pos);
		qDebug() << ":GuiBoardView:" << "calling of listener clickedCell";
	}
}

void GuiBoardView::moveMaked(const Move& move) {
	redrawBoardFigures = true;
	this->update();
}
