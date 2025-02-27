/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/27 16:36:57 by florencecou      ###   ########.fr       */
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
		int server_fdsocket;
		static bool Signal; //-> static boolean for signal
		std::vector<struct pollfd> fdTable; //-> vector of pollfd
		std::vector<Client> clients;
		struct sockaddr_in cliadd;
		struct pollfd new_cli;
		
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