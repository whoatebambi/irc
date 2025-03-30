#ifdef __linux__

#pragma once

#include "Poller.hpp"

#include <sys/epoll.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

class EpollPoller : public Poller {
private:
	int	_epollFd;

public:
	EpollPoller();
	~EpollPoller();

	bool	add(int fd);
	bool	remove(int fd);
	void	unregisterFd(int fd);
	int		wait(PollEvent* outEvents, int maxEvents, int timeout);
	int		getFd() const;
};

#endif // __linux__