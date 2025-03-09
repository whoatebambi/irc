/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/09 16:03:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <csignal>




static void signal_interrupt(int sig)
{
    (void)sig;
    Server::getInstance().shutdown();
}

static int handle_signals(int sig, void (*handler)(int))
{
    sigset_t set;
    struct sigaction action;

    sigemptyset(&set);
    action.sa_handler = handler;
    action.sa_mask = set;
    action.sa_flags = 0;
    if (sigaction(sig, &action, NULL) == -1)
    {
        throw std::runtime_error("Sigaction failed");
        return -1;
    }
    return 0;
}

int	main(void)
{
	try {
		handle_signals(SIGINT, &signal_interrupt);
        handle_signals(SIGQUIT, SIG_IGN);
		Server::getInstance().Init();
		while(Server::getInstance().isLive())
			Server::getInstance().Monitor();
	}
	catch(const std::exception &e) {
		// Server::getInstance().CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	
	return (0);
}

