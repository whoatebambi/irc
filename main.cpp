#include "Server.hpp"

void handleSignal(int signal)
{
	(void)signal;
	// std::cout << std::endl << "Signal " << signal << " (SIGINT) reçu. Fermeture propre du programme." << std::endl;
	Server::getInstance().shutdown();
	return ;
}

int	main(int argc, char **argv)
{
	try {
		std::signal(SIGINT, handleSignal);
		if (argc > 1)
			Server::getInstance().Init(argv);
		else
			Server::getInstance().Init();
		while(Server::getInstance().isLive())
			Server::getInstance().Monitor();
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}
