// move to root for testing!!!

#include "PollerFactory.hpp"
#include "PollEvent.hpp"
#include <iostream>
#include <unistd.h>

int main()
{
	Poller* poller = NULL;

	try {
		poller = createPoller();
	} catch (const std::exception& e) {
		std::cerr << "Failed to create poller: " << e.what() << std::endl;
		return 1;
	}

	if (!poller->add(0)) // stdin
	{
		std::cerr << "Failed to add fd 0 to poller" << std::endl;
		delete poller;
		return 1;
	}

	std::cout << "Poller is now waiting for input on STDIN (fd 0)..." << std::endl;

	while (true)
	{
		PollEvent events[1];
		int count = poller->wait(events, 1, 5000); // 5 second timeout

		if (count == -1)
		{
			std::cerr << "Poller wait failed" << std::endl;
			break;
		}
		else if (count == 0)
		{
			std::cout << "[Timeout] No input within 5 seconds." << std::endl;
		}
		else
		{
			if (events[0].fd == 0 && events[0].readable)
			{
				char buffer[128];
				ssize_t n = read(0, buffer, sizeof(buffer) - 1);
				if (n > 0)
				{
					buffer[n] = '\0';
					std::cout << "[Input] " << buffer;
				}
			}
		}
	}

	delete poller;
	return 0;
}