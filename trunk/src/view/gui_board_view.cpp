#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include <algorithm>
#include <cassert>

#include "gui_board_view.h"

#include <QBoxLayout>
#include <QListWidget>
#include <QPushButton>

GuiBoardView::GuiBoardView(QWidget *parent)
: QWidget(parent) {

    setupUi();

    myModel = 0;

    myActivePlayer = 0;
    myCatch = this->NO_CATCH;
}

void GuiBoardView::setupUi() {

    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    this->setSizePolicy(sizePolicy);
}

QSize GuiBoardView::minimumSizeHint() const {
    return QSize(100, 100);
}

int GuiBoardView::heightForWidth(int width) const {
    return width;
}

void GuiBoardView::countSizes() {

    assert(myModel != 0);

    QSize screenSize = this->size();
    QSize boardSize(myModel->getBoardSizeX(), myModel->getBoardSizeY());

    int cellWidth = screenSize.width() / boardSize.width();
    int cellHeight = screenSize.height() / boardSize.height();
    int cellSize = std::min(cellWidth, cellHeight);

    myCellSize = QSize(cellSize, cellSize);

    myBoardSize = QSize(myCellSize.width() * boardSize.width(), myCellSize.height() * boardSize.height());

    qDebug() << ":GuiBoardView:" << "recounted size:: "
            << myCellSize << myBoardSize;


}

void GuiBoardView::loadFiguresGraphic() {
    assert(myModel != 0);

    qDebug() << ":GuiBoardView: " << "loadBoardFigures()";
    for (FIGURES_DATA::const_iterator itFigure = myModel->getAllFiguresData().begin();
            itFigure != myModel->getAllFiguresData().end();
            ++itFigure) {

        for (int i = 0; i != 2; ++i) {

            QString pictureFile = itFigure->second.picture[i].c_str();
            char letter = itFigure->second.letter;
            if (pictureFile == "") {
                QString name = ":/images/pieces/";
                name += (i == WHITE) ? "white" : "black";
                name += figureLetterToName(letter);
                name += ".svg";
                pictureFile = name;
            }


            QPixmap picture(pictureFile);
            myPicturesFigure[i][letter] = picture;
        }

    }
}

void GuiBoardView::scalingFigures() {


    qDebug() << ":GuiBoardView: " << "scalingFigures()";

    for (int i = 0; i != 2; ++i) {
        myPicturesFigureScaled[i].clear();
    }
    PICTURES_FIGURES::iterator it;
    for (int i = 0; i != 2; ++i) {
        for (it = myPicturesFigure[i].begin(); it != myPicturesFigure[i].end(); ++it) {
            qDebug() << ":GuiBoardView: " << "scalingFigures() " << "scaling" << it->first;
            myPicturesFigureScaled[i][it->first] = it->second.scaled(myCellSize, Qt::KeepAspectRatio);
        }
    }

}

QString GuiBoardView::figureLetterToName(char letter) {
    QString name;
    switch (letter) {
        case 'K':
            name = "King";
            break;
        case 'Q':
            name = "Queen";
            break;
        case 'R':
            name = "Rook";
            break;
        case 'N':
            name = "Knight";
            break;
        case 'B':
            name = "Bishop";
            break;
        case 'P':
            name = "Pawn";
            break;
        default:
            name = "Default";
            break;
    }
    return name;
}

void GuiBoardView::prepareBoardCells() {


    qDebug() << ":GuiBoardView: " << "prepareBoardCells() ";

    QColor whiteCellColor(255, 247, 153),
            blackCellColor(140, 98, 57);


    myBoardCells = QPixmap(myBoardSize);
    QPainter painter(&myBoardCells);

    //board drawing
    painter.setPen(Qt::NoPen);
    myBoardCells.fill(whiteCellColor);
    painter.setBrush(blackCellColor);
    for (int i = 0; i != myBoardSize.height(); ++i) {
        for (int j = 0; j != myBoardSize.width(); ++j) {
            if ((i + j) % 2 == 0) {
                painter.drawRect(QRect(QPoint(myCellSize.width() * j, myCellSize.height() * i), myCellSize));
            }
        }
    }


}

void GuiBoardView::prepareFigures() {



    qDebug() << ":GuiBoardView: " << "prepareFigures()";

    myBoardFigures = QPixmap(myBoardSize);
    myBoardFigures.fill(Qt::transparent);
    QPainter painter(&myBoardFigures);

    FIGURES::const_iterator itFigure;
    for (int i = 0; i < 2; ++i) {
        for (itFigure = myModel->getSetFigures(i).begin(); itFigure != myModel->getSetFigures(i).end(); ++itFigure) {
            if (itFigure->captured == false) {
                //                qDebug() << ":GuiBoardView: " << "prepareFigures(): "
                //                        << (i == 0 ? "white" : "black")
                //                        << "id: " << itFigure->id
                //                        << "position: x: " << itFigure->position.myX << " y: " << itFigure->position.myY
                //                        << myModel->getFigureData(itFigure->id).picture[WHITE].c_str()
                //                        << myModel->getFigureData(itFigure->id).picture[BLACK].c_str();
                //qDebug() << ":GuiBoardView: " << "prepareFigures(): " << "asking for letter of figure id: " << itFigure->id << "player " << i;
                char letter = myModel->getFigureData(itFigure->id).letter;
                PICTURES_FIGURES::const_iterator it = myPicturesFigureScaled[i].find(letter);
                assert(it != myPicturesFigureScaled[i].end());
                QPoint point(myCellSize.width() * itFigure->position.myX, myCellSize.height() * itFigure->position.myY);
                painter.drawPixmap(point, it->second);
            }
        }
    }

}

void GuiBoardView::prepareMovesHighlights() {

    qDebug() << ":GuiBoardView: " << "prepareHighlights()";
    myBoardMovesHighlights = QPixmap(myBoardSize);
    myBoardMovesHighlights.fill(Qt::transparent);
    QPainter painter(&myBoardMovesHighlights);
    painter.setPen(Qt::NoPen);

    Position pos;
    QColor color;
    for (HIGHLIGHT_CELLS::iterator itHighlight = myMovesHighlights.begin(); itHighlight != myMovesHighlights.end(); ++itHighlight) {
        pos = itHighlight->first;
        color = itHighlight->second;
        painter.setBrush(color);
        painter.drawRect(QRect(QPoint(myCellSize.width() * pos.myX, myCellSize.height() * pos.myY), myCellSize));
        //painter.drawEllipse(QRect(QPoint(myCellSize.width() * pos.myX, myCellSize.height() * pos.myY), myCellSize));

    }

}

void GuiBoardView::prepareUnderCaptureHighlights() {

    qDebug() << ":GuiBoardView: " << "prepareUnderCaptureHighlights()";
    myBoardUnderCaptureHighlights = QPixmap(myBoardSize);
    myBoardUnderCaptureHighlights.fill(Qt::transparent);
    QPainter painter(&myBoardUnderCaptureHighlights);
    painter.setPen(Qt::NoPen);


    assert(myModel != 0);
    myUnderCaptureHighlights.clear();
    MOVES moves = myModel->allMoves(1 - myModel->getCurrentPlayer(), CAPTURE);
    MOVES::const_iterator itMove;
    for (itMove = moves.begin(); itMove != moves.end(); ++itMove) {
        QColor color(255, 0, 0, 128);
        myUnderCaptureHighlights.push_back(std::make_pair(itMove->pos2, color));
    }

    Position pos;
    QColor color;
    for (HIGHLIGHT_CELLS::iterator itHighlight = myUnderCaptureHighlights.begin(); itHighlight != myUnderCaptureHighlights.end(); ++itHighlight) {
        pos = itHighlight->first;
        color = itHighlight->second;
        painter.setBrush(color);
        painter.drawRect(QRect(QPoint(myCellSize.width() * pos.myX, myCellSize.height() * pos.myY), myCellSize));
    }
}

void GuiBoardView::setModel(Model* model) {
    myModel = model;
    loadFiguresGraphic();
    countSizes();
    prepareBoardCells();
    scalingFigures();
    prepareFigures();
    this->update();
}

void GuiBoardView::resizeEvent(QResizeEvent*) {

    qDebug() << ":GuiBoardView: " << "resizeEvent()";

    if (myModel == 0) {
        qDebug() << ":GuiBoardView: " << "resizeEvent()" << "model is not setted";
        return;
    }

    countSizes();
    scalingFigures();
    prepareBoardCells();
    prepareFigures();
    prepareMovesHighlights();
    prepareUnderCaptureHighlights();
}

void GuiBoardView::paintEvent(QPaintEvent*) {

    if (myModel == 0) {
        qDebug() << ":GuiBoardView:" << "paintEvent() " << "model is not setted";
        return;
    }

    qDebug() << ":GuiBoardView: " << "paintEvent() " << "---rendering of board view is started---";
    QPainter painter(this);
    QPoint leftTopEdge(0, 0);


    //it should doing in updateIt function
    // some bugs with thread, that didn't end;
    prepareUnderCaptureHighlights();
    prepareFigures();
    //////////////////////////////////////

    painter.drawPixmap(leftTopEdge, myBoardCells);
    painter.drawPixmap(leftTopEdge, myBoardUnderCaptureHighlights);
    painter.drawPixmap(leftTopEdge, myBoardMovesHighlights);
    painter.drawPixmap(leftTopEdge, myBoardFigures);
    qDebug() << ":GuiBoardView: " << "paintEvent() " << "---rendering of board view is finished---";

}

void GuiBoardView::mousePressEvent(QMouseEvent *pe) {
    if (pe->buttons() & Qt::LeftButton) {

        if (myModel != 0) {
            Position pos;
            pos.myX = pe->x() / myCellSize.width();
            pos.myY = pe->y() / myCellSize.height();
            qDebug() << ":GuiBoardView:" << "CELL IS " << pos.myX << pos.myY;
            catchCell(pos);
        }
    }
}

GuiBoardView::~GuiBoardView() {
}

void GuiBoardView::updateIt() {
    this->update();
}

void GuiBoardView::setActivePlayer(Player* player) {
    myActivePlayer = player;

    if (myActivePlayer == 0) {
        myCatch = this->NO_CATCH;
        return;
    }
    myCatch = this->CATCH_START_CELL;

}

void GuiBoardView::catchCell(const Position& pos) {

    if (myActivePlayer == 0) {
        myCatch = this->NO_CATCH;
        return; //no cathcing, if no active player
    }

    if (myCatch == this->CATCH_START_CELL) {
        catchStartCell(pos);
    } else if (myCatch == this->CATCH_FINISH_CELL) {
        catchFinishCell(pos);
    }
}

void GuiBoardView::catchStartCell(const Position& pos) {
    assert(myActivePlayer != 0);



    if (myModel->getBoardCell(pos.myX, pos.myY).player != myActivePlayer->getColor()) {
        qDebug() << ":GuiBoardView:" << "catched start cell for not active player ";
        return;
    };

    qDebug() << ":GuiBoardView:" << "catched start cell for active player: " << myActivePlayer->getColor();




    MOVES moves = myModel->movesFromPosition(pos);
    if (moves.empty()) {
        return;
    }

    mySimpleMove.pos1 = pos;
    myCatch = this->CATCH_FINISH_CELL;

    myMovesHighlights.clear();
    MOVES::const_iterator itMove;
    for (itMove = moves.begin(); itMove != moves.end(); ++itMove) {
        QColor color = itMove->type & MOVE ? Qt::cyan : Qt::red;
        myMovesHighlights.push_back(std::make_pair(itMove->pos2, color));
    }

    prepareMovesHighlights();
    this->update();
}

void GuiBoardView::catchFinishCell(const Position& pos) {

    qDebug() << ":GuiBoardView:" << "catched finish cell";

    myMovesHighlights.clear();
    prepareMovesHighlights();
    this->update();

    mySimpleMove.pos2 = pos;
    if (myModel->canMove(mySimpleMove) == false) {
        myCatch = this->CATCH_START_CELL;
        return;
    }



    myCatch = this->NO_CATCH;

    myActivePlayer->moveFromBoard(mySimpleMove);

}

