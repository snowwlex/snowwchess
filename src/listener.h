/*
 * listener.h
 *
 *  Created on: 23.04.2010
 *      Author: snowwlex
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include "snowwchess.h"

class Listener {
	public:
		virtual void moveReady(const Move&)      { qDebug() << ":Listener: call of listener moveReady"; }
		virtual void pushedCell(const Position&) { qDebug() << ":Listener: call of listener pushedCell"; }
		virtual void moveMaked(const Move&) { qDebug() << ":Listener: call of listener moveMaked"; }
	protected:
		Listener() { }
};


#endif /* LISTENER_H_ */
