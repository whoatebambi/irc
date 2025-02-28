/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/28 17:35:08 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::Signal = false; //-> initialize the static boolean

Server::Server(){this->server_fd = -1;} // Server::Server() : server_fd(-1) {}
Server::~Server(){}
Server::Server(Server const &src){*this = src;}
Server &Server::operator=(Server const &src){
	if (this != &src){
		this->port = src.port;
		this->server_fd = src.server_fd;
		this->clientsTable = src.clientsTable;
		this->fdTable = src.fdTable;
	}
	return (*this);
}

void Server::SignalHandler(int signum)
{
	(void)signum;
	// std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true; //-> set the static boolean to true to stop the server
}


void Server::CloseFds() {
	for(size_t i = 0; i < clientsTable.size(); i++){
		std::cout << "Client <" << clientsTable[i].GetFd() << "> Disconnected" << std::endl;
		close(clientsTable[i].GetFd());
	}
	if (server_fd != -1) {
		std::cout << "Server <" << server_fd << "> disconnected" << std::endl;
		close(server_fd);
	}
}

// 1️⃣ Create the socket → socket()
// 2️⃣ Set socket options → setsockopt() (prevents "Address already in use" error)
// 3️⃣ Set non-blocking mode → fcntl()
// 4️⃣ Bind the socket to specified address/port → bind()
// 5️⃣ Mark the socket for listening → listen()
void Server::InitSocket() {
	std::cout << "Initializing server...\n";
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0); // socket() creates the server socket
    if (server_fd == -1)
        throw(std::runtime_error("socket() failed"));
	
    sockaddr_in serverAddr; // Internet domain socket setup
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // Sets the address family to ipv4
    serverAddr.sin_port = htons(port); // Converts the port to network byte order (big endian)
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Means “listen on all available network interfaces.”

	int en = 1;
	// By default, when a server closes a socket, the operating system keeps the port blocked for a short period 
	// Without SO_REUSEADDR, when the server is restarted, it might fail to bind for a while.
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("setsockopt() SO_REUSEADDR failed"));
	// OPTIONAL // SO_KEEPALIVE Prevents keeping dead connections open forever
	// setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
	// OPTIONAL // TCP_NODELAY Disables Nagle's Algorithm (faster small messages)
	// setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	
	// Sets the socket to non-blocking mode. Socket operations (accept(), recv(), etc.)
	// will return immediately instead of waiting.
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) 
		throw(std::runtime_error("fcntl() O_NONBLOCK failed"));

	// OPTIONAL // SIGPIPE (Prevents crashes when writing to a closed socket)
	// signal(SIGPIPE, SIG_IGN);
	// Associates server_fd with a specific IP address and port stored in serverAddr.
	// This allows clientsTable to know where to connect.
    if (bind(server_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw(std::runtime_error("bind() failed"));
	// Reasons for failure: port already in use, socket not created properly,
	//  address not available (e.g. requires admin privileges).
	std::cout << "TEST\n";

	// Tells the OS that this socket is now ready to accept 128 connections
	// (SOMAXCONN is the max amount allowed per kernel).
    if (listen(server_fd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));
		// Reasons for failure: socket not properly bound, system is out of resources.

	newClient.fd = server_fd;
	newClient.events = POLLIN;
	newClient.revents = 0;
	fdTable.push_back(newClient);
}

void Server::InitPollFd() {
	pollfd NewPoll;

	NewPoll.fd = this->server_fd; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	fdTable.push_back(NewPoll); //-> add the server socket to the pollfd
}

void Server::start() {
	this->port = 4444;
    
	InitSocket();
	InitPollFd();
	std::cout << "Server initialized\n";
    std::cout << "Server <" << this->server_fd << "> listening on port " << port << "\n";
	
	while (Server::Signal == false) //-> run the server until the signal is received
	{
		if((poll(&fdTable[0],fdTable.size(),-1) == -1) && Server::Signal == false) //-> wait for an event
			throw(std::runtime_error("poll() failed"));
		for (size_t i = 0; i < fdTable.size(); i++) //-> check all file descriptors
		{
			if (fdTable[i].revents & POLLIN)//-> check if there is data to read
			{
				if (fdTable[i].fd == this->server_fd)
					AcceptNewClient(); //-> accept new client
				else
					ReceiveNewData(fdTable[i].fd);
			}
		}
	}
	// for (size_t i = 0; i < fdTable.size(); ++i) {
	// 	std::cout << "\n─── fdTable[" << i << "]\n";
	// 	std::cout << "\t├── fd\t\t" << fdTable[i].fd << "\n";
	// 	std::cout << "\t├── events\t" << fdTable[i].events << "\n";
	// 	std::cout << "\t└── revents\t" << fdTable[i].revents << "\n";
	// }
	CloseFds(); // Close the file descriptors when the server stops
}

void Server::AcceptNewClient()
{
	Client clientObject; // Create a new client object
	std::memset(&cliadd, 0, sizeof(cliadd)); // Initialize client address structure to zero
	socklen_t len = sizeof(cliadd); // Store the size of the client address structure

	// accept() extracts the 1st connection request from the pending queue, creating a new socket
	int fdClient = accept(server_fd, (sockaddr *)&(cliadd), &len);
	if (fdClient == -1)
	{ std::cout << "accept() failed" << std::endl; return; }

	// fcntl() edits the fd and sets the new socket to non-blocking mode
	if (fcntl(fdClient, F_SETFL, O_NONBLOCK) == -1)
	{ std::cout << "fcntl() failed" << std::endl; return; }

	this->newClient.fd = fdClient; // Store the client's file descriptor
	this->newClient.events = POLLIN; // Set the event type for poll(). POLLIN: The client socket is ready for reading
	this->newClient.revents = 0;// Reset the revents field (used by poll() to report events)

	clientObject.SetFd(fdClient); // Store the file descriptor in the client object
	clientObject.setIpAdd(inet_ntoa((cliadd.sin_addr))); // Convert the client's IP address from binary to string format and store it

	clientsTable.push_back(clientObject); // Add the new client to the list of connected clientsTable
	fdTable.push_back(newClient); // Add the client’s file descriptor info to the poll() table

	std::cout << "Client <" << fdClient << "> Connected" << std::endl;
}

void	Server::ReceiveNewData(int fd) {	
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
	if(bytes <= 0)
	{
		RemoveClient(fd);
		RemoveFds(fd);
		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
		close(fd);
	}
}

void Server::RemoveClient(int fd){
	for (size_t i = 0; i < this->clientsTable.size(); i++){
		if (this->clientsTable[i].GetFd() == fd)
			{
				this->clientsTable.erase(this->clientsTable.begin() + i);
				return;
			}
	}
}

void Server::RemoveFds(int fd){
	for (size_t i = 0; i < this->fdTable.size(); i++){
		if (this->fdTable[i].fd == fd)
			{
				this->fdTable.erase(this->fdTable.begin() + i);
				return;
			}
	}
}
