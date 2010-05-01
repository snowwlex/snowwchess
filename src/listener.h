#ifndef LISTENER_H_
#define LISTENER_H_

#include "snowwchess.h"

class Listener {
	public:
		virtual void moveReady(const Move&)      { qDebug() << ":Listener: call of listener moveReady"; }
		virtual void pushedCell(const Position&) { qDebug() << ":Listener: call of listener pushedCell"; }
		virtual void turnMaked(const Move&) { qDebug() << ":Listener: call of listener turnMaked"; }
		virtual void stop() { qDebug() << ":Listener: call of listener stop"; }
		virtual void start() { qDebug() << ":Listener: call of listener start"; }
		virtual void updateIt() { qDebug() << ":Listener: call of listener updateIt"; }
	protected:
		Listener() { }
};


#endif /* LISTENER_H_ */
