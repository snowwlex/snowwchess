#include "../model/game.h"
#include "human_player.h"

HumanPlayer::HumanPlayer() {
	
}

void HumanPlayer::moveFromBoard(const SimpleMove& move) {
    Game::instance()->moveReady(myColor, move);
}

void HumanPlayer::getMove() {

}


