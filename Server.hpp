/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/20 16:26:15 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>

class Server {
	private:
		int _port;
		int _socketFd;
		// static bool Signal; //-> static boolean for signal
		std::vector<struct pollfd> fds; //-> vector of pollfd
		
	public:
		Server();
		~Server();
		void start();

		void	CloseFds();
		void	InitSocket();
		void	InitPollFd();
		// static void SignalHandler(int signum); //-> signal handler
};