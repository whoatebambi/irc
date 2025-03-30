#ifdef __linux__

#ifndef EPOLLPOLLER_HPP
#define EPOLLPOLLER_HPP

#include "Poller.hpp"
#include <sys/epoll.h>
#include <unistd.h>

class EpollPoller : public Poller {
private:
	int _epollFd;

public:
	EpollPoller();
	~EpollPoller();

	bool add(int fd);
	bool remove(int fd);
	int	wait(PollEvent* outEvents, int maxEvents, int timeout);
	int getFd() const;
};

#endif // EPOLLPOLLER_HPP

#endif // __linux__