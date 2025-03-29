#pragma once

#include "Client.hpp"
#include "Reply.hpp"

#include "Command.hpp"
#include "CommandCap.hpp"
#include "CommandNick.hpp"
#include "CommandUser.hpp"
#include "CommandMode.hpp"
#include "CommandJoin.hpp"
#include "CommandPass.hpp"
#include "CommandPing.hpp"
#include "CommandPrivMsg.hpp"
#include "CommandTopic.hpp"
#include "CommandPart.hpp"
#include "CommandInvite.hpp"
#include "CommandKick.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <vector>
#include <arpa/inet.h> 
#include <csignal>
#include <cstdlib>
#include <map>
#include <set>

#define INVERSE "\033[7m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define INVERSE_BG "\033[7;49m"
#define BOLD "\033[1m"

class Command;
class CommandCap;
class CommandNick;
class CommandUser;
class CommandMode;
class CommandJoin;
class CommandPass;
class CommandPing;
class CommandTopic;
class CommandPart;
class CommandInvite;
class CommandKick;

class Client;
class Channel;

class Server
{
	private:
		int			_fd;
		std::string	_serverName;
		bool		_live; // need static?
		int			_port;
		std::string _pass;
		std::map<std::string, Command*>	_CommandMap;
	
		struct sockaddr_in	_cliadd;
		int					_epollFd;
		std::vector<Client*>	_clientVec;
		std::set<Channel*>		_channelSet;

	public:
		Server();
		~Server();

		// Initialize the server
		static	Server &getInstance();
		void	init(); // still needed?
		void	init(char **argv);
		void	initCommandMap();

		// Closing functions
		void	shutdown();
		void	closeFds();

		// Server getters/setters
		const std::string	&get_serverName() const;
		bool				is_live() const;
		const std::string	&get_pass() const;
		const std::map<std::string, Command*> &get_CommandMap() const;

		// Server data handling
		void	monitor();
		void	acceptNewClient();
		void	handleDataClient(const epoll_event &event);
		void	addChannel(Client *client, std::string name, std::string key);
		void	removeClient(int fd);
		
		// Clients and channels setters/getters
		bool	isClient(const std::string &nickname) const;
		bool	isRegisteredClient(const std::string &nickname) const;
		Client	*get_client(std::string const &nickname);
		const std::vector<Client*>	&get_clientVec() const;
		const std::set<Channel*>	&get_channelSet() const;
};
