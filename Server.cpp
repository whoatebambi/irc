/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/27 16:36:59 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::Signal = false; //-> initialize the static boolean

Server::Server(){this->server_fdsocket = -1;} // Server::Server() : server_fdsocket(-1) {}
Server::~Server(){}
Server::Server(Server const &src){*this = src;}
Server &Server::operator=(Server const &src){
	if (this != &src){
		this->port = src.port;
		this->server_fdsocket = src.server_fdsocket;
		this->clients = src.clients;
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
	for(size_t i = 0; i < clients.size(); i++){
		std::cout << "Client <" << clients[i].GetFd() << "> Disconnected" << std::endl;
		close(clients[i].GetFd());
	}
	if (server_fdsocket != -1) {
		std::cout << "Server <" << server_fdsocket << "> disconnected" << std::endl;
		close(server_fdsocket);
	}
}

// 1️⃣ Create the socket → socket()
// 2️⃣ Set socket options → setsockopt() (prevents "Address already in use" error)
// 3️⃣ Set non-blocking mode → fcntl()
// 4️⃣ Bind the socket to specified address/port → bind()
// 5️⃣ Mark the socket for listening → listen()
void Server::InitSocket() {
	std::cout << "Initializing server...\n";
	
	server_fdsocket = socket(AF_INET, SOCK_STREAM, 0); // socket() creates the server socket
    if (server_fdsocket == -1)
        throw(std::runtime_error("Socket creation failed"));
	
    sockaddr_in serverAddr; // Internet domain socket setup
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // Sets the address family to ipv4
    serverAddr.sin_port = htons(port); // Converts the port to network byte order (big endian)
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Means “listen on all available network interfaces.”

	int en = 1;
	// By default, when a server closes a socket, the operating system keeps the port blocked for a short period 
	// Without SO_REUSEADDR, when the server is restarted, it might fail to bind for a while.
	if(setsockopt(server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	// OPTIONAL // SO_KEEPALIVE Prevents keeping dead connections open forever
	// setsockopt(server_fdsocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
	// OPTIONAL // TCP_NODELAY Disables Nagle's Algorithm (faster small messages)
	// setsockopt(server_fdsocket, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	
	// Sets the socket to non-blocking mode. Socket operations (accept(), recv(), etc.)
	// will return immediately instead of waiting.
	if (fcntl(server_fdsocket, F_SETFL, O_NONBLOCK) == -1) 
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
		
	// OPTIONAL // SIGPIPE (Prevents crashes when writing to a closed socket)
	// signal(SIGPIPE, SIG_IGN);

	// Associates server_fdsocket with a specific IP address and port stored in serverAddr.
	// This allows clients to know where to connect.
    if (bind(server_fdsocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw(std::runtime_error("Bind failed"));
		// Reasons for failure: port already in use, socket not created properly,
		//  address not available (e.g. requires admin privileges).

	// Tells the OS that this socket is now ready to accept 128 connections
	// (SOMAXCONN is the max amount allowed per kernel).
    if (listen(server_fdsocket, SOMAXCONN) == -1)
		throw(std::runtime_error("Listen failed"));
		// Reasons for failure: socket not properly bound, system is out of resources.

	new_cli.fd = server_fdsocket;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	fdTable.push_back(new_cli);
}

void Server::InitPollFd() {
	pollfd NewPoll;

	NewPoll.fd = this->server_fdsocket; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	fdTable.push_back(NewPoll); //-> add the server socket to the pollfd
}

void Server::start() {
	this->port = 4444;
    
	InitSocket();
	InitPollFd();
	std::cout << "Server initialized\n";
    std::cout << "Server <" << this->server_fdsocket << "> listening on port " << port << "\n";
	
	while (Server::Signal == false) //-> run the server until the signal is received
	{
		if((poll(&fdTable[0],fdTable.size(),-1) == -1) && Server::Signal == false) //-> wait for an event
		throw(std::runtime_error("poll() faild"));
		for (size_t i = 0; i < fdTable.size(); i++) //-> check all file descriptors
		{
			if (fdTable[i].revents & POLLIN)//-> check if there is data to read
			{
				if (fdTable[i].fd == server_fdsocket)
					AcceptNewClient(); //-> accept new client
				else
					ReceiveNewData(fdTable[i].fd);
					// {ReceiveNewData(fdTable[i].fd); CloseFds(); return;} //-> receive new data from a registered client
			}
		}
	}
	// for (size_t i = 0; i < fdTable.size(); ++i) {
	// 	std::cout << "\n─── fdTable[" << i << "]\n";
	// 	std::cout << "\t├── fd\t\t" << fdTable[i].fd << "\n";
	// 	std::cout << "\t├── events\t" << fdTable[i].events << "\n";
	// 	std::cout << "\t└── revents\t" << fdTable[i].revents << "\n";
	// }
	CloseFds(); //-> close the file descriptors when the server stops
}

void Server::AcceptNewClient()
{
	Client cli;
	std::memset(&cliadd, 0, sizeof(cliadd));
	socklen_t len = sizeof(cliadd);
	int incofd = accept(server_fdsocket, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
	new_cli.fd = incofd;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	cli.SetFd(incofd);
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr)));
	clients.push_back(cli);
	fdTable.push_back(new_cli);
	std::cout << "Client <" << incofd << "> Connected" << std::endl;
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
	std::cout << "RemoveClient()\n";
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			{
				this->clients.erase(this->clients.begin() + i);
				return;
			}
	}
}

void Server::RemoveFds(int fd){
	std::cout << "RemoveFds()\n";
	for (size_t i = 0; i < this->fdTable.size(); i++){
		if (this->fdTable[i].fd == fd)
			{
				this->fdTable.erase(this->fdTable.begin() + i);
				return;
			}
	}
}
