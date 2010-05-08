#ifndef GUIBOARDVIEW_H_
#define GUIBOARDVIEW_H_

#include <QtGui/QWidget>
#include <QPainter>
#include <QPixmap>

#include "../snowwchess.h"
#include "../model/model.h"
#include "../subscriber.h"
#include "../player/player.h"

class GuiBoardView : public QWidget, public Subscriber {
    Q_OBJECT

public:
    GuiBoardView(QWidget *parent = 0);
    ~GuiBoardView();

public:
    void setModel(Model* model);
    void setActivePlayer(Player* player);

public: //listener methods
    virtual void updateIt();

public: //ui methods
    virtual QSize minimumSizeHint() const;
    virtual int heightForWidth(int width) const;

protected: //events
    virtual void paintEvent(QPaintEvent *pe);
    virtual void mousePressEvent(QMouseEvent *me);
    virtual void resizeEvent(QResizeEvent* re);

private: //draw preparing methods
    void countSizes();
    void scalingFigures();
    void prepareBoardCells();
    void prepareFigures();
    void prepareMovesHighlights();
    void prepareUnderCaptureHighlights();

private:
    void loadFiguresGraphic();

private:
    void catchCell(const Position& pos);
    void catchStartCell(const Position& pos);
    void catchFinishCell(const Position& pos);

private: //help methods
    QString figureLetterToName(char letter);
   

private: //ui methods
    void setupUi();


private: //state of view

    enum CATCHING {
        NO_CATCH,
        CATCH_START_CELL,
        CATCH_FINISH_CELL
    };

private:
    typedef std::map<char, QPixmap> PICTURES_FIGURES;
    typedef std::vector< std::pair<Position, QColor> > HIGHLIGHT_CELLS;
private: //fields
    Model* myModel;
    QPixmap myBoardCells;
    QPixmap myBoardFigures;
    QPixmap myBoardMovesHighlights;
    QPixmap myBoardUnderCaptureHighlights;
    HIGHLIGHT_CELLS myMovesHighlights;
    HIGHLIGHT_CELLS myUnderCaptureHighlights;

    PICTURES_FIGURES myPicturesFigureScaled[2];
    PICTURES_FIGURES myPicturesFigure[2];

    QSize myCellSize;
    QSize myBoardSize;


    Player* myActivePlayer;
    CATCHING myCatch;
    SimpleMove mySimpleMove;


private: //ui objects


};


#endif /* GUIBOARDVIEW_H_ */
