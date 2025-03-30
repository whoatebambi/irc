#pragma once

#include "PollEvent.hpp"

class Poller
{
	public:
		virtual ~Poller() {}

		virtual bool add(int fd) = 0;
		virtual bool remove(int fd) = 0;
		virtual void unregisterFd(int fd) = 0;
		virtual int wait(PollEvent* outEvents, int maxEvents, int timeout) = 0;
};
