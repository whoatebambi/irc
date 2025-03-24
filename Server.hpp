#pragma once

#include "Client.hpp"
#include "NumericReplies.hpp"

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
		int	_port;
		int	_fd;
		std::string	_serverName;
		std::string _pass;
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
		std::vector<Client *> getClients() const;
		std::string getPass() const;
		void setPass(std::string newPass);
		bool	isLive();
		void	shutdown();
		void	Monitor();
		void	CloseFds();
		void	Init();
		void	Init(char **argv);
		void	AcceptNewClient();
		void	RemoveClient(int fd);
		void	handleClientData(const epoll_event &event);
		void	handleEpollWaitError();
		void	handleEpollError(const epoll_event &event);

		void	newChannel(Client *client, std::string chanName, std::string key);

		std::string	getServerName() const;
		std::map<std::string, Channel*>	getChannelMap() const;
};
