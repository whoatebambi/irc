/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/28 17:27:59 by florencecou      ###   ########.fr       */
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
#include <poll.h>
#include <vector>
#include <arpa/inet.h> //-> for inet_ntoa()

class Server {
	private:
		int port;
		int server_fd;
		static bool Signal; // static boolean for signal
		std::vector<struct pollfd> fdTable; // Stores only the file descriptors needed for poll() (fd, events, revents)
		std::vector<Client> clientsTable; // Stores detailed client information (fd, ip, nickname, messages, status...)
		struct sockaddr_in cliadd;
		struct pollfd newClient;

		
		
	public:
		Server();
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &src);
		
		void start();
		void	CloseFds();
		void	InitSocket();
		void	InitPollFd();
		static void	SignalHandler(int signum); //-> signal handler
		void	AcceptNewClient();
		void	ReceiveNewData(int fd);
		void	RemoveClient(int fd);
		void	RemoveFds(int fd);
};