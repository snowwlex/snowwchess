/*
 * sender.h
 *
 *  Created on: 23.04.2010
 *      Author: snowwlex
 */

#ifndef SENDER_H_
#define SENDER_H_

#include "snowwchess.h"
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
		Sender(const Sender& sender) { }
		Sender& operator=(const Sender& sender) { }

	protected:

		LISTENERS myListeners;

};

#endif /* SENDER_H_ */
