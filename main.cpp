#include "Server.hpp"

void handleSignal(int signal)
{
	(void)signal;
	Server::getInstance().shutdown();
	return ;
}

int	main(int argc, char **argv)
{
	if (argc != 3) 
		return (std::cout << "./ircserv [port] [password]" << std::endl, 1);
	try {
		std::signal(SIGINT, handleSignal);
		Server::getInstance().init(argv[1], argv[2]);
		while(Server::getInstance().is_running())
			Server::getInstance().monitor();
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}
