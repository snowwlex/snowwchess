#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include "snowwchess.h"

class Subscriber {
	public:
		virtual void updateIt() { qDebug() << ":Subscriber: call of  updateIt"; }
	protected:
		Subscriber() { }
};


#endif /*  SUBSCRIBER_H_ */
