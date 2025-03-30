#pragma once

#include "Poller.hpp"

#include <vector>
#include <poll.h>
#include <map>
#include <cstddef>
#include <iostream>
#include <unistd.h>
#include <cstring>

class PollPoller : public Poller
{
	private:
		std::vector<struct pollfd>	_fds;
		std::map<int, size_t>		_fdIndexMap;

	public:
		PollPoller();
		virtual ~PollPoller();

		bool	add(int fd);
		bool	remove(int fd);
		void	unregisterFd(int fd);
		int		wait(PollEvent* outEvents, int maxEvents, int timeout);
};
