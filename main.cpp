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
	// if (argc != 3) 
	// {
	(void)argc;
	(void)argv;
	// const std::string port = "4444";
	// const std::string password = "1234";
	// }
	// if argv[1] or argv[2] check fails
		// return (1);
	try {
		std::signal(SIGINT, handleSignal);
		// Server::getInstance().init(argv[1], argv[2]);
		Server::getInstance().init("6667", "1234");
		while(Server::getInstance().is_running())
			Server::getInstance().monitor();
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}
