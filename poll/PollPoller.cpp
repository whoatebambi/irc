#include "PollPoller.hpp"

PollPoller::PollPoller() {}

PollPoller::~PollPoller() {}

bool PollPoller::add(int fd)
{
	// Prevent duplicates
	if (_fdIndexMap.find(fd) != _fdIndexMap.end())
		return false;

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_fds.push_back(pfd);
	_fdIndexMap[fd] = _fds.size() - 1;
	return true;
}

bool PollPoller::remove(int fd)
{
	std::map<int, size_t>::iterator it = _fdIndexMap.find(fd);
	if (it == _fdIndexMap.end())
		return false;

	size_t index = it->second;
	size_t lastIndex = _fds.size() - 1;

	if (index != lastIndex)
	{
		_fds[index] = _fds[lastIndex];
		_fdIndexMap[_fds[index].fd] = index;
	}
	_fds.pop_back();
	_fdIndexMap.erase(it);
	return true;
}

void PollPoller::unregisterFd(int fd)
{
	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_fds.erase(it);
			break;
		}
	}
}

int PollPoller::wait(PollEvent* outEvents, int maxEvents, int timeout)
{
	int n = poll(&_fds[0], _fds.size(), timeout);
	if (n <= 0) return n;

	int count = 0;
	for (size_t i = 0; i < _fds.size() && count < maxEvents; ++i)
	{
		if (_fds[i].revents & (POLLIN | POLLERR | POLLHUP))
		{
			PollEvent e;
			e.fd = _fds[i].fd;
			e.readable = _fds[i].revents & POLLIN;
			e.error = _fds[i].revents & (POLLERR | POLLHUP);
			outEvents[count++] = e;
		}
	}
	return count;
}