#ifndef POLLER_HPP
#define POLLER_HPP

#include "PollEvent.hpp"

class Poller {
public:
	virtual ~Poller() {}

	virtual bool add(int fd) = 0;
	virtual bool remove(int fd) = 0;

	// Fill 'outEvents' with up to maxEvents events
	virtual int wait(PollEvent* outEvents, int maxEvents, int timeout) = 0;
};

#endif