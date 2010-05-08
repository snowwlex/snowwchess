#include <cassert>
#include <QBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStringList>
#include <QMessageBox>

#include "gui_history_view.h"
#include "../model/game.h"

GuiHistoryView::GuiHistoryView(QWidget *parent)
: QWidget(parent) {
    setupUi();

    myHistory = 0;

}

void GuiHistoryView::setupUi() {

    // LAYOUTS
    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    this->setLayout(boxLayout);

    //Table Widget
    listWidget = new QListWidget();
    boxLayout->addWidget(listWidget);

    listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listWidget->setAutoScroll(true);

    // Undo Button
    undoButton = new QPushButton();
    undoButton->setText("Make undo");
    boxLayout->addWidget(undoButton);



    connect(undoButton, SIGNAL(clicked()), this, SLOT(makeUndo()));
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(makeUndo()));
}

QSize GuiHistoryView::sizeHint() const {
    return QSize(250, 0);
}

void GuiHistoryView::setHistory(History* history) {
    myHistory = history;
    updateIt();
}

GuiHistoryView::~GuiHistoryView() {

}

void GuiHistoryView::makeUndo() {
    qDebug() << ":GuiHistoryView: " << "current row is " << listWidget->currentRow();
    int row = listWidget->currentRow();
    if (row == -1) {
        QMessageBox(QMessageBox::NoIcon, "Sorry", "select a move to undo to", QMessageBox::Ok).exec();
        return;
    }

    int undoQty = myHistory->getMoves().size() - row;
    bool ok = myHistory->makeUndo(undoQty);
    assert(ok == true);
    Game::instance()->changeModel(myHistory->getLastModel());


}

QString GuiHistoryView::moveToString(const Move& move) {
    int sizeY = Game::instance()->getModel().getBoardSizeY();
    char letter1 = move.pos1.myX + 'a';
    char letter2 = move.pos2.myX + 'a';
    char digit1 = sizeY - move.pos1.myY + '0';
    char digit2 = sizeY - move.pos2.myY + '0';
    return QString("%1%2-%3%4").arg(letter1).arg(digit1).arg(letter2).arg(digit2);
}

void GuiHistoryView::updateIt() {
    qDebug() << ":GuiHistoryView: has listened updateIt()";
    assert(myHistory != 0);
    listWidget->clear();
    listWidget->addItem("Initial Position");
    const MOVES& moves = myHistory->getMoves();

    if (moves.size() > 0) {
        QStringList allMovesDescribes;
        for (MOVES::const_iterator itMove = moves.begin(); itMove != moves.end(); ++itMove) {
            QString playerName = itMove->player == WHITE ? "W" : "B";
            QString figureName = Game::instance()->getModel().getFigureData(itMove->figureId).name.c_str();
            QString movesDescribe = playerName + " " + figureName + " " + makeComment(*itMove) + " " + moveToString(*itMove);
            allMovesDescribes.append(movesDescribe);
        }
        listWidget->addItems(allMovesDescribes);
    }

}

QString GuiHistoryView::makeComment(const Move& move) {

    QString figureName;
    QString comment;

    switch (move.type) {
        case CAPTURE:
        case INPASSING:

            figureName = Game::instance()->getModel().getFigureData(move.figureCapturedId).name.c_str();
            comment = "captures " + figureName;
            break;
        case MOVE:
            comment = "moves";
            break;
    }


    switch (move.effect) {
        case CASTLE:
            comment = "castles";
    }
    return comment;
}