/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/20 13:01:43 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <csignal>

int	main(void)
{
	Server server;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		// signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		// signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		server.start(); //-> initialize the server
	}
	catch(const std::exception& e){
		server.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}

