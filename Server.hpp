#pragma once

#include "Client.hpp"
#include "Reply.hpp"
#include "poll/PollerFactory.hpp"
#include "poll/Poller.hpp"
#include "poll/EpollPoller.hpp"
#include "poll/PollPoller.hpp"

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
#include "CommandQuit.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
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

class Client;
class Channel;

class Server
{
	private:
		Poller* _poller;
		int			_fd;
		std::string	_serverName;
		bool		_running;
		int			_port;
		std::string _password;
		std::map<std::string, Command*>	_commandMap;
	
		struct sockaddr_in	_cliadd;
		std::vector<Client*>	_clientVec;
		std::set<Channel*>		_channelSet;

		static const int MIN_PORT = 1024;
		static const int MAX_PORT = 65535;

	public:
		// Initialize the server
		Server();
		static Server	&getInstance();
		void	init(const std::string &port, const std::string &password);
		void	createServerSocket();
		void	bindAndListen();
		void	setupPoller();
		void	addServerSocketToPoller();
		void	initCommandMap();
		
		// Closing functions
		~Server();
		void	shutdown();
		void	closeFds();

		// Server getters/setters
		const std::string	&get_serverName() const;
		bool				is_running() const;
		const std::string	&get_password() const;
		const std::map<std::string, Command*> &get_CommandMap() const;

		// Server data handling
		void	monitor();
		void	handlePollEvent(const PollEvent& event);
		void	acceptNewClient();
		int		acceptSocketClient();
		void	createAndStoreClient(int clientFd);
		void	handleDataClient(int fd);
		void	addChannel(Client *client, const std::string &name, const std::string &key);
		void	removeChannel(Channel *channel);
		void	removeClient(int fd);
		
		// Clients and channels setters/getters
		bool	isClient(const std::string &nickname) const;
		bool	isRegisteredClient(const std::string &nickname) const;
		Client	*get_client(const std::string &nickname);
		std::set<int>	getSharedMembersFd(Client *client);
		const std::vector<Client*>	&get_clientVec() const;
		const std::set<Channel*>	&get_channelSet() const;
};
