/*
 * guiboardview.h
 *
 *  Created on: 10.04.2010
 *      Author: snowwlex
 */

#ifndef GUIBOARDVIEW_H_
#define GUIBOARDVIEW_H_

#include <QtGui/QWidget>
#include <QPainter>
#include "model.h"

class GuiBoardView : public QWidget {
	Q_OBJECT

	public:
		GuiBoardView(QWidget *parent = 0);
		~GuiBoardView();

	public:
		void setModel(Model* model);

	public:

	public slots:

	private slots:

	signals:

	protected: //events
		virtual void paintEvent(QPaintEvent *pe);
		virtual void mouseMoveEvent(QMouseEvent *pe);

	private: //draw methods
		void drawBoardCells(QPainter& painter);
		void drawFigures(QPainter& painter);

	private: //fields
		Model* myModel;

};


#endif /* GUIBOARDVIEW_H_ */
