/*
 * guiboardview.cpp
 *
 *  Created on: 10.04.2010
 *      Author: snowwlex
 */

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "guiboardview.h"

GuiBoardView::GuiBoardView(QWidget *parent)
    : QWidget(parent)
{
	myModel = 0;
}

void GuiBoardView::drawBoardCells(QPainter& painter) {

	QColor  whiteCellColor(255,247,153),
			blackCellColor(140,98,57);

	int boardSizeX = myModel->getBoardSizeX();
	int boardSizeY = myModel->getBoardSizeY();
	int screenWidth = this->width();
	int screenHeight = this->height();

	// black color background
	painter.setBrush(Qt::black);

	painter.drawRect( QRect( QPoint(0,0), this->size() ));


	//calculating size of cell
	int leastSize = screenWidth < screenHeight ? screenWidth : screenHeight;
	int cellWidth = leastSize / boardSizeX;
	int cellHeight = leastSize / boardSizeY;
	QSize cellSize(cellWidth, cellHeight);


	//board drawing
	painter.setPen(Qt::NoPen);
	painter.setBrush(whiteCellColor);
	painter.drawRect( QRect( QPoint(0,0), QSize(cellWidth*boardSizeX,cellHeight*boardSizeY)) );
	painter.setBrush(blackCellColor);

	for (int i = 0; i != boardSizeY; ++i) {
		for (int j = 0; j != boardSizeX; ++j) {
			if ( (i+j)%2 == 0) {
				painter.drawRect( QRect( QPoint(cellWidth*j,cellHeight*i), cellSize ) );
			}
		}
	}

}



void GuiBoardView::drawFigures(QPainter& painter) {

	int boardSizeX = myModel->getBoardSizeX();
	int boardSizeY = myModel->getBoardSizeY();
	int screenWidth = this->width();
	int screenHeight = this->height();

	int leastSize = screenWidth < screenHeight ? screenWidth : screenHeight;
	int cellWidth = leastSize / boardSizeX;
	int cellHeight = leastSize / boardSizeY;

	FIGURES::const_iterator itFigure;
	for (int i=0; i<2; ++i) {
		for ( itFigure=myModel->getSetFigures(i).begin(); itFigure != myModel->getSetFigures(i).end(); ++itFigure ) {
			if (itFigure->captured == false) {
//				qDebug() << (i==0? "white":"black")
//						 << "id: " << itFigure->id
//						 << "position: x: " << itFigure->position.myX << " y: " << itFigure->position.myY
//						 << myModel->getFigureData(itFigure->id).picture[WHITE].c_str()
//						 << myModel->getFigureData(itFigure->id).picture[BLACK].c_str();

				QPixmap picture(myModel->getFigureData(itFigure->id).picture[i].c_str());
				QPoint point(cellWidth * itFigure->position.myX, cellHeight * itFigure->position.myY);
//				int pictureWidth = picture.width();
//				int pictureHeight = picture.height();
//				qreal scaleX = (qreal)cellWidth / pictureWidth;
//				qreal scaleY = (qreal)cellHeight / pictureHeight;

				picture = picture.scaledToWidth(cellWidth);
				painter.drawPixmap(point,picture);
			}
		}
	}
}

void GuiBoardView::setModel(Model* model) {
	myModel = model;
}
void GuiBoardView::paintEvent(QPaintEvent *pe) {
	qDebug() << "paint event rendering";

	if (myModel == 0) {
		qDebug() << "model is not setted";
		return;
	}

	QPainter painter(this);
	drawBoardCells(painter);
	drawFigures(painter);
	qDebug() << "rendering of board view finished";

}

void GuiBoardView::mouseMoveEvent(QMouseEvent *pe) {
	qDebug() << pe->x() << pe->y();
	if ( pe->buttons() & Qt::LeftButton) {
		qDebug() << "pressed";
	}
}
GuiBoardView::~GuiBoardView() {

}
