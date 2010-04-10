/*
 * tester.h
 *
 *  Created on: 10.04.2010
 *      Author: snowwlex
 */

#ifndef TESTER_H_
#define TESTER_H_

class aiTester {
	private:
		AIPlayer* myPlayer;
	public:
	aiTester(AIPlayer* player) : myPlayer(player) { }
	void operator()(int number) {
		Move move;
		myPlayer->setDepth(number);
		myPlayer->makeTurn(NONE);
	}
};

#endif /* TESTER_H_ */
