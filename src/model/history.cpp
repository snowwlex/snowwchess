#include "history.h"

bool History::makeUndo(int quantityMoves) {
    int i;
    for (i = 0; i != quantityMoves && myMoves.empty() == false; ++i) {
        myMoves.pop_back();
        myModels.pop_back();
    }
    notifyUpdateIt();
    if (i != quantityMoves) {
        return false;
    }
    return true;
}

History::History(const Model& initModel) {
    myInitModel = new Model(initModel);
}

History::~History() {
    delete myInitModel;
}

const Model& History::getLastModel() const{
    if (myModels.empty() == true) {
        return *myInitModel;
    }
    return myModels.back();
}

void History::notifyUpdateIt() const {
    for (SUBSCRIBERS::const_iterator itListener = mySubscribers.begin(); itListener != mySubscribers.end(); ++itListener) {
        (*itListener)->updateIt();
        qDebug() << ":History:" << "notifyUpdateIt()";
    }
}

void History::addRecord(const Move & move, const Model & model) {
    myMoves.push_back(move);
    myModels.push_back(model);

    notifyUpdateIt();
}

void History::clear() {
    myMoves.clear();
    myModels.clear();
}

const MOVES& History::getMoves() const {
    return myMoves;
}


