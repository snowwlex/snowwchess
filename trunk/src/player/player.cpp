#include "player.h"
#include "../model/game.h"


int Player::getColor() {
    return myColor;
}

void Player::setModel(Model* model) {
    myModel = model;
}

void Player::setColor(int color) {
    myColor = color;
}



