
#ifndef SENDER_H_
#define SENDER_H_

#include <set>
#include "subscriber.h"

class Publisher {
	public:
		void addSubscriber(Subscriber* subscriber) {
			mySubscribers.insert(subscriber);
		}
		void removeSubscriber(Subscriber* subscriber) {
			mySubscribers.erase(subscriber);
		}

	protected:
		typedef std::set< Subscriber* > SUBSCRIBERS;

	protected:
		Publisher() { }
		Publisher(const Publisher&) { }
		Publisher& operator=(const Publisher&) { return *this; }

	protected:

		SUBSCRIBERS mySubscribers;

};

#endif /* SENDER_H_ */
