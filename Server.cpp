/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/06 16:37:58 by jpointil         ###   ########.fr       */
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
		// this->fdTable = src.fdTable;
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
		std::cout << "Client <" << clientsTable[i].getFd() << "> Disconnected" << std::endl;
		close(clientsTable[i].getFd());
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

	// Tells the OS that this socket is now ready to accept 128 connections
	// (SOMAXCONN is the max amount allowed per kernel).
    if (listen(server_fd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));
		// Reasons for failure: socket not properly bound, system is out of resources.
}

void Server::InitEpoll() {
    epoll_fd = epoll_create1(0); // Create an epoll instance
    if (epoll_fd == -1) {
		throw(std::runtime_error("epoll_create1() failed"));}
	struct epoll_event event;
	// memset
    event.events = EPOLLIN; // or EPOLLIN | EPOLLRDHUP; to check
    event.data.fd = this->server_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->server_fd, &event) == -1) // Add the server socket to the epoll instance
        throw(std::runtime_error("epoll_ctl() failed"));
}

void Server::start() {
	this->port = 4444;
    
	InitSocket();
	InitEpoll();
	std::cout << "Server initialized\n";
    std::cout << "Server <" << this->server_fd << "> listening on port " << port << "\n";
	struct epoll_event events[42];
	while (Server::Signal == false) // Run the server until the signal is received
    {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // Wait for an event
        if (event_count == -1 && Server::Signal == false)
            throw(std::runtime_error("epoll_wait() failed"));

        for (int i = 0; i < event_count; i++) // Check all file descriptors
        {
            if (events[i].events & EPOLLIN) // Check if there is data to read
            {
                if (events[i].data.fd == this->server_fd)
                    AcceptNewClient(); // Accept new client
                else
                    ReceiveNewData(events[i].data.fd);
            }
        }
    }
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
	{
		std::cout << "accept() failed" << std::endl;
		return;
	}

	// fcntl() edits the fd and sets the new socket to non-blocking mode
	if (fcntl(fdClient, F_SETFL, O_NONBLOCK) == -1)
	{ std::cout << "fcntl() failed" << std::endl; return; }

	struct epoll_event event;
    event.events = EPOLLIN; // Set the event type for epoll(). EPOLLIN: The client socket is ready for reading
    event.data.fd = fdClient;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fdClient, &event) == -1) // Add the client socket to the epoll instance
    {
        std::cout << "epoll_ctl() failed" << std::endl;
        close(fdClient);
        return;
    }

    clientObject.SetFd(fdClient); // Store the file descriptor in the client object
    clientObject.setIpAdd(inet_ntoa((cliadd.sin_addr))); // Convert the client's IP address from binary to string format and store it

    clientsTable.push_back(clientObject); // Add the new client to the list of connected clients

	std::cout << "Client <" << fdClient << "> Connected" << std::endl;
}

void	Server::ReceiveNewData(int fd) {	
	char buffer[1024];
	std::string line;
	ssize_t bytes;

	// catch errors
	
	memset(buffer, 0, sizeof(buffer));

	size_t i = 0;
	while (i < clientsTable.size())
	{
		if (clientsTable[i].getFd() == fd && !clientsTable[i].getSaved().empty()) {
			line += clientsTable[i].getSaved();
			clientsTable[i].getSaved().clear();
			// std::cout << "FOUND MATCHING FD =  <" << fd << std::endl;
			break;
		}
		i++;
	}

	bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);
	line += buffer;

	if (line.empty() || bytes == -1)//if(bytes <= 0)
	{
		RemoveClient(fd);
		RemoveFds(fd);
		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
		close(fd);
	}

	// check if in list

	size_t pos;
    do
    {
        pos = line.find("\r\n");
        if (pos != std::string::npos)
            pos += 2;

        std::string lineRead = line.substr(0, pos);
        line = line.erase(0, pos);

        if (pos == std::string::npos && !lineRead.empty())
        {
            clientsTable[i].getSaved() = lineRead;
            return;
        }
        if (!lineRead.empty())
        {
            std::cout << "CLIENT " << fd << ">> " << lineRead;
			// check if in list
            parse(lineRead);
        }
    } while (pos != std::string::npos);

	std::cout << "PRINT line =  " << line << std::endl;

}

std::string Server::ft_trim(const std::string &str)
{
    size_t end = str.find_last_not_of("\r\n");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

void Server::parse(std::string &line)
{
	std::cout << "line =  " << line << std::endl;
    size_t pos = line.find(' ');
    std::string cmd = line.substr(0, pos);

    std::string args;
    if (pos != std::string::npos) 
        args = ft_trim(line.substr(pos + 1));
    else 
        args = "";

    if (cmd == "CAP")
	{
		std::cout << "FOUND COMMAND =  " << cmd << std::endl;
		// sendMsg(client, "CAP * LS :");
	}
    else
        std::cout << "Unknown command: " << cmd << std::endl;
}


void Server::RemoveClient(int fd){
	for (size_t i = 0; i < this->clientsTable.size(); i++){
		if (this->clientsTable[i].getFd() == fd)
			{
				this->clientsTable.erase(this->clientsTable.begin() + i);
				return;
			}
	}
}

void Server::RemoveFds(int fd)
{
	// for (size_t i = 0; i < this->fdTable.size(); i++){
	// 	if (this->fdTable[i].fd == fd)
	// 		{
	// 			this->fdTable.erase(this->fdTable.begin() + i);
	// 			return;
	// 		}
	// }
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "epoll_ctl() failed to remove fd: " << fd << std::endl;
    }
}
