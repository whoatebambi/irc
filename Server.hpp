/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/09 17:20:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include <arpa/inet.h> //-> for inet_ntoa()

class Client; // Forward declaration of Client class

class Server {
	private:
		int port;
		int _fd;
		bool _isLive; // need static?
		std::vector<Client> clientsTable; // Stores detailed client information (fd, ip, nickname, messages, status...)
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
		// void	RemoveFds(int fd);

};