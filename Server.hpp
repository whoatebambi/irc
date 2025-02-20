/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/20 12:17:38 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Server {
	public:
		Server();
		~Server();
		void start();

		void CloseFds();

		// static void SignalHandler(int signum); //-> signal handler
	
	private:
		int _port;
		int _socketFd;
		// static bool Signal; //-> static boolean for signal
};