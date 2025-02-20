/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/20 13:19:06 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

// bool Server::Signal = false; //-> initialize the static boolean

// void Server::SignalHandler(int signum)
// {
// 	(void)signum;
// 	std::cout << std::endl << "Signal Received!" << std::endl;
// 	Server::Signal = true; //-> set the static boolean to true to stop the server
// }

Server::Server() : _socketFd(-1) {}

Server::~Server() {
	if (_socketFd != -1)
	close(_socketFd);
}

void Server::CloseFds() {
	if (_socketFd != -1) {
		std::cout << "Server <" << _socketFd << "> Disconnected" << std::endl;
		close(_socketFd);
	}
}

// 1️⃣ Create the socket → socket()
// 2️⃣ Set socket options → setsockopt() (prevents "Address already in use" error)
// 3️⃣ Set non-blocking mode → fcntl()
// 4️⃣ Bind the socket to specified address/port → bind()
// 5️⃣ Mark the socket for listening → listen()
void Server::start() {
	_port = 4444;
    _socketFd = socket(AF_INET, SOCK_STREAM, 0); // socket() creates the server socket
    if (_socketFd == -1)
        throw(std::runtime_error("Socket creation failed"));
	
    sockaddr_in serverAddr; // Internet domain socket setup
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // Sets the address family to ipv4
    serverAddr.sin_port = htons(_port); // Converts the port to network byte order (big endian)
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Means “listen on all available network interfaces.”

	// std::cout << "\n─── server\n";
    // std::cout << "\t├── _port\t" << _port << "\n";
    // std::cout << "\t└── _socketFd\t" << _socketFd << "\n";
    // std::cout << "\n─── serverAddr\n";
    // std::cout << "\t├── sin_family\t" << "AF_INET" << serverAddr.sin_family << "\n";
    // std::cout << "\t├── sin_port\t" << ntohs(serverAddr.sin_port) << "\n";
    // std::cout << "\t└── sin_addr\t" << "\n";
    // std::cout << "\t    └── s_addr\t" << serverAddr.sin_addr.s_addr << "\n\n";

	int en = 1;
	// By default, when a server closes a socket, the operating system keeps the port blocked for a short period 
	// Without SO_REUSEADDR, when the server is restarted, it might fail to bind for a while.
	if(setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	// OPTIONAL // SO_KEEPALIVE Prevents keeping dead connections open forever
	// setsockopt(_socketFd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
	// OPTIONAL // TCP_NODELAY Disables Nagle's Algorithm (faster small messages)
	// setsockopt(_socketFd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	
	// Sets the socket to non-blocking mode. Socket operations (accept(), recv(), etc.)
	// will return immediately instead of waiting.
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1) 
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
		
	// OPTIONAL // SIGPIPE (Prevents crashes when writing to a closed socket)
	// signal(SIGPIPE, SIG_IGN);

	// Associates _socketFd with a specific IP address and port stored in serverAddr.
	// This allows clients to know where to connect.
    if (bind(_socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		// Reasons for failure: port already in use, socket not created properly,
		//  address not available (e.g. requires admin privileges).
		throw(std::runtime_error("Bind failed"));
    }

	// Tells the OS that this socket is now ready to accept 128 connections
	// (SOMAXCONN is the max amount allowed per kernel).
    if (listen(_socketFd, SOMAXCONN) == -1) {
		// Reasons for failure: socket not properly bound, system is out of resources.
		throw(std::runtime_error("Listen failed"));
    }

    std::cout << "Server listening on port " << _port << "\n";
    while (true);
}