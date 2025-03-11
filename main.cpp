#include "Server.hpp"

#include <iostream>
#include <csignal>
#include <cstdlib>

void handleSignal(int signal) {
    
    std::cout << std::endl << "Signal " << signal << " (SIGINT) reçu. Fermeture propre du programme." << std::endl;
    // std::exit(signal);
    Server::getInstance().shutdown();
    return ;
}

int	main(void)
{
	try {
        std::signal(SIGINT, handleSignal);
		Server::getInstance().Init();
		while(Server::getInstance().isLive())
			Server::getInstance().Monitor();
	}
	catch(const std::exception &e) {
		// Server::getInstance().CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}
