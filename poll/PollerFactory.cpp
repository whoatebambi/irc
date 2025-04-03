#include "PollerFactory.hpp"

#ifdef __linux__
	#include "EpollPoller.hpp"
#else
	#include "PollPoller.hpp"
#endif

Poller* createPoller()
{
#ifdef __linux__
	return new EpollPoller();
#else
	return new PollPoller();
#endif
}
