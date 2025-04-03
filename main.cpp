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
	int _port = atoi(argv[1]);
	if (_port < MIN_PORT || _port > MAX_PORT)
		return (std::cout << "Invalid port: must be between 1024 and 65535" << std::endl, 1);
	if (!isValidPasword(argv[2]))
		return (std::cout << "Invalid password" << std::endl, 1);
		
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
