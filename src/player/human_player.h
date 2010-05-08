
#ifndef HUMAN_PLAYER_H_
#define HUMAN_PLAYER_H_

#include "player.h"

class HumanPlayer : public Player {

	public:
		HumanPlayer();
                virtual void moveFromBoard(const SimpleMove& move);

	public: 
		virtual void getMove();
            


	public: //variables
		
		
};


#endif /* HUMAN_PLAYER_H_ */
