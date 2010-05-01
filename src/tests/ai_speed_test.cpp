#include "ai_speed_test.h"

AISpeedTest::AISpeedTest() {
	myPlayer = 0;
}

void AISpeedTest::setPlayer(AIPlayer* aiPlayer) {
	myPlayer = aiPlayer;
}

AISpeedTest::~AISpeedTest() {
}

void AISpeedTest::testing() {
	myPlayer->makeTurn();
}

void AISpeedTest::moveReady(const Move&) {
	testingComplete = true;
}


