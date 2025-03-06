/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/06 16:24:08 by jpointil         ###   ########.fr       */
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

class Server {
	private:
		int port;
		int server_fd;
		static bool Signal; // static boolean for signal
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
		
		void start();
		void	CloseFds();
		void	InitSocket();
		void	InitEpoll();
		static void	SignalHandler(int signum); //-> signal handler
		void	AcceptNewClient();
		void	ReceiveNewData(int fd);
		void	RemoveClient(int fd);
		void	RemoveFds(int fd);

		std::string	ft_trim(const std::string &str);
		void		parse(std::string &line);
};