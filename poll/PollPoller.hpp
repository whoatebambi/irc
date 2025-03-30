#ifndef POLLPOLLER_HPP
#define POLLPOLLER_HPP

#include "Poller.hpp"
#include <vector>
#include <poll.h>
#include <map>
#include <cstddef> // for size_t

class PollPoller : public Poller {
private:
	std::vector<struct pollfd> _fds;
	std::map<int, size_t> _fdIndexMap; // Maps fd → index in _fds

public:
	PollPoller();
	virtual ~PollPoller();

	bool add(int fd);
	bool remove(int fd);
	int	wait(PollEvent* outEvents, int maxEvents, int timeout);
};

#endif