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

class Client;

class Server {
	private:
		int port;
		int _fd;
		bool _isLive; // need static?
		std::vector<Client*> clientsTable;
		struct sockaddr_in cliadd;
		int epoll_fd;
		static const int MAX_EVENTS = 10;
		// epoll_event events[MAX_EVENTS];

	public:
		Server();
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &src);

		static Server& getInstance();
		bool isLive();
		void shutdown();
		
		void Monitor();
		void	CloseFds();
		void	Init();
		void	AcceptNewClient();
		void	RemoveClient(int fd);
		void handleEpollIn(const epoll_event &event);
		void  handleClientData(const epoll_event &event);
		void  handleEpollWaitError();
		void  handleEpollError(const epoll_event &event);
};