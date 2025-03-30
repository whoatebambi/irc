#ifdef __linux__

#include "EpollPoller.hpp"
#include <stdexcept>
#include <iostream>

EpollPoller::EpollPoller()
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1)
		throw std::runtime_error("epoll_create1() failed");
}

EpollPoller::~EpollPoller()
{
	close(_epollFd);
}

bool EpollPoller::add(int fd)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	return epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event) != -1;
}

bool EpollPoller::remove(int fd)
{
	return epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) != -1;
}

// int EpollPoller::wait(void* events, int maxEvents, int timeout)
// {
// 	return epoll_wait(_epollFd, static_cast<struct epoll_event*>(events), maxEvents, timeout);
// }
int EpollPoller::wait(PollEvent* outEvents, int maxEvents, int timeout)
{
	struct epoll_event rawEvents[64];
	int n = epoll_wait(_epollFd, rawEvents, 64, timeout);
	if (n <= 0) return n;

	int count = 0;
	for (int i = 0; i < n && count < maxEvents; ++i)
	{
		PollEvent e;
		e.fd = rawEvents[i].data.fd;
		e.readable = rawEvents[i].events & EPOLLIN;
		e.error = rawEvents[i].events & (EPOLLERR | EPOLLHUP);
		outEvents[count++] = e;
	}
	return count;
}

int EpollPoller::getFd() const
{
	return _epollFd;
}

#endif // __linux__