#ifndef GUIBOARDVIEW_H_
#define GUIBOARDVIEW_H_

#include <QtGui/QWidget>
#include <QPainter>
#include <QPixmap>

#include "../snowwchess.h"
#include "../model/model.h"
#include "../sender.h"

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
		virtual void turnMaked(const Move& move);
		virtual void updateIt();

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
		void loadBoardFigures();

	private: //help methods
		QString figureLetterToName(char letter);

	private: //fields
		Model* myModel;
		QPixmap myBoardCells;
		QPixmap myBoardFigures;
		QPixmap myBoardHighlightCells;
		HIGHLIGHT_CELLS myHighlightCells;

		std::map<char, QPixmap> picturesFigure[2];

		QSize boardSize;
		QSize screenSize;
		QSize cellSize;
		bool redrawBoardCells,
			 redrawBoardFigures,
			 redrawBoardHighlightCells;
		bool boardFiguresLoaded;



};


#endif /* GUIBOARDVIEW_H_ */
