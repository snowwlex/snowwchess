#ifndef PLAYER_H_
#define PLAYER_H_

#include "../model/model.h"

class Player {

	public: // turn making methods methods
                virtual void moveFromBoard(const SimpleMove& ) { qDebug() << ":Player:" << "moveFromBoard()"; }
 		virtual void getMove() = 0;
                virtual void stopGetMove() { qDebug() << ":Player:" << "stopGetMove() "; }

        public: //getters
                int getColor();

	public: //setters
		void setModel(Model* model);
		void setColor(int color);

	protected:
		int myColor;
		const Model *myModel;

};






#endif /* PLAYER_H_ */
