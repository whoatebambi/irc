#pragma once

struct PollEvent
{
	int fd;
	bool readable;
	bool error;
};
