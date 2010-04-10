/*
 * player.h
 *
 *
 *      Author: snowwlex
 */

#ifndef PLAYER_H_
#define PLAYER_H_


class Player {
	public:
		void setModel(Model* model) { myModel = model; }
		void setColor(int color)    { myColor = color; }

	public:
		virtual Move makeTurn(GameMessage message = NONE) = 0;

	protected:
		int myColor;
		Model *myModel;

};


class HumanPlayer : public Player {

	public:
		HumanPlayer();

	public:
		virtual Move makeTurn(GameMessage message = NONE);
};




#endif /* PLAYER_H_ */
