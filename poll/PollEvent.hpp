#ifndef POLLEVENT_HPP
#define POLLEVENT_HPP

struct PollEvent {
	int fd;
	bool readable;
	bool error;
};

#endif