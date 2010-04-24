/*
 * sender.h
 *
 *  Created on: 23.04.2010
 *      Author: snowwlex
 */

#ifndef SENDER_H_
#define SENDER_H_

#include "listener.h"

class Sender {
	public:
		void addListener(Listener* listener) {
			myListeners.push_back(listener);
		}
		void removeListener(Listener* listener) {
			myListeners.remove(listener);
		}

	protected:
		typedef std::list< Listener* > LISTENERS;

	protected:
		Sender() { }
		Sender(const Sender&) { }
		Sender& operator=(const Sender&) { return *this; }

	protected:

		LISTENERS myListeners;

};

#endif /* SENDER_H_ */
