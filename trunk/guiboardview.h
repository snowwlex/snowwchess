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
#include "sender.h"

typedef std::vector< std::pair<Position, QColor> > HIGHLIGHT_CELLS;

class GuiBoardView : public QWidget, public Sender, public Listener {
	Q_OBJECT

	public:
		GuiBoardView(QWidget *parent = 0);
		~GuiBoardView();

	public:
		void setModel(Model* model);

	public:
		void highlight(const HIGHLIGHT_CELLS& highlightCells);
		void clearHightlights();

	public: //listener methods
		virtual void moveMaked(const Move& move);

	public slots:

	private slots:

	signals:

	private: //sender methods
		void notifyClickedCell(const Position& pos) const;




	protected: //events
		virtual void paintEvent(QPaintEvent *pe);
		virtual void mousePressEvent(QMouseEvent *pe);

	private: //draw preparing methods
		void prepareBoardCells();
		void prepareFigures();
		void prepareHighlightCells();

	private:
		void countSizes();

	private: //fields
		Model* myModel;
		QPixmap myBoardCells;
		QPixmap myBoardFigures;
		QPixmap myBoardHighlightCells;
		HIGHLIGHT_CELLS myHighlightCells;

		QSize boardSize;
		QSize screenSize;
		QSize cellSize;
		bool redrawBoardCells,
			 redrawBoardFigures,
			 redrawBoardHighlightCells;



};


#endif /* GUIBOARDVIEW_H_ */
