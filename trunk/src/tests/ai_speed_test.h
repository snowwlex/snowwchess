
#ifndef AI_SPEED_TEST_H_
#define AI_SPEED_TEST_H_

#include "../snowwchess.h"
#include "time_test.h"
#include "../subscriber.h"
#include "../player/ai_player.h"

int makeAISpeedTest();

class AISpeedTest : public TimeTest, public Subscriber {

	public:
		AISpeedTest();
		virtual ~AISpeedTest();

	public:
		void setPlayer(AIPlayer* aiPlayer);

	public:
		virtual void moveReady(const Move&);

	protected:
		virtual void testing();

	private:
		AIPlayer* myPlayer;

};

#endif /* AI_SPEED_TEST_H_ */
