#ifndef HISTORY_H_
#define HISTORY_H_

#include <vector>

#include "../snowwchess.h"
#include "model.h"
#include "../publisher.h"

class History : public Publisher {
public:
    History(const Model& initModel);
    ~History();
public:
    void addRecord(const Move& move, const Model& model);
    const MOVES& getMoves() const;
    const Model& getLastModel() const;
    bool makeUndo(int quantityMoves = 1);
    void clear();

private: //sender methods
    void notifyUpdateIt() const;

private:
    MOVES myMoves;
    std::vector< Model > myModels;
    Model* myInitModel;

};
#endif /* HISTORY_H_ */
