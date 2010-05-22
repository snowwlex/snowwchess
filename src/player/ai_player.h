/*
 * ai.h
 *
 *  Created on: 09.03.2010
 *      Author: snowwlex
 */

#ifndef AI_H_
#define AI_H_

#include "../snowwchess.h"
#include "player.h"

class AIPlayer : public Player {
public:
    virtual void getMove() = 0;

public: //setters
    void setDepth(int depth);
    void setAddDepth(int addDepth);

    void setABEqualPruning(bool b) {
        abEqualPruning = b;
    }

protected:

    AIPlayer() {
        myTurnsCounter = 0;
        abEqualPruning = false;
    }

protected:
    int sefMaterial(const Model& model, int player) const;

protected:
    int myDepth;
    int myAddDepth;
    bool abEqualPruning;

protected: //for debug
    int myTurnsCounter;
    int myCounter;
    int myQCounter;


};

class sortMVV_LVA {
public:

    sortMVV_LVA(const Model* model) : myModel(model) {
    }
    bool operator()(const Move& move1, const Move& move2);
private:
    const Model* myModel;
};




#endif /* AI_H_ */
