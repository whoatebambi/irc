#pragma once

#include "Client.hpp"

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

class Client;
class Channel;

class Server
{
	private:
		int	port;
		int	_fd;
		std::string	_serverName;
		bool	_isLive; // need static?
		std::vector<Client*>	clientsTable;
		struct sockaddr_in	cliadd;
		int	epoll_fd;
		static const int	MAX_EVENTS = 10;
		std::map<std::string, Channel*> _channelMap;

	public:
		Server();
		~Server();

		static	Server& getInstance();
		bool	isLive();
		void	shutdown();
		void	Monitor();
		void	CloseFds();
		void	Init();
		void	AcceptNewClient();
		void	RemoveClient(int fd);
		void	handleClientData(const epoll_event &event);
		void	handleEpollWaitError();
		void	handleEpollError(const epoll_event &event);

		void	newChannel(Client *client, std::string chanName, std::string key);

		std::string	getServerName() const;
};
