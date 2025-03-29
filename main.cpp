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
			Server::getInstance().init(argv);
		else
			Server::getInstance().init();
		while(Server::getInstance().is_live())
			Server::getInstance().monitor();
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}
