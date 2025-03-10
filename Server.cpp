/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/10 14:06:21 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

bool Server::isLive()
{
    if(this->_isLive)
        return true;
    return false;
}

void Server::shutdown()
{
    this->_isLive = false;
}

Server::Server(){this->_fd = -1;} // Server::Server() : _fd(-1) {}
Server::~Server(){}
Server::Server(Server const &src){*this = src;}
Server &Server::operator=(Server const &src){
	if (this != &src){
		this->port = src.port;
		this->_fd = src._fd;
		this->clientsTable = src.clientsTable;
		// this->fdTable = src.fdTable;
	}
	return (*this);
}



void Server::CloseFds() {
	for(size_t i = 0; i < clientsTable.size(); i++){
		std::cout << "Client <" << clientsTable[i].getFd() << "> Disconnected" << std::endl;
		close(clientsTable[i].getFd());
	}
	if (_fd != -1) {
		std::cout << "Server <" << _fd << "> disconnected" << std::endl;
		close(_fd);
	}
}

void Server::Init() {
	this->_isLive = true;
	this->port = 4444;
	this->_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
	if (this->_fd == -1)
		throw std::runtime_error("socket() failed");

	int opt = 1; // Set socket options
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed");

	sockaddr_in server_addr; // Bind the socket to an address and port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->port);
	if (bind(this->_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("bind() failed");
		
	if (listen(this->_fd, SOMAXCONN) == -1) // Listen for incoming connections
		throw std::runtime_error("listen() failed");

	this->epoll_fd = epoll_create1(0); // Create an epoll instance
	if (this->epoll_fd == -1)
		throw std::runtime_error("epoll_create1() failed");

	struct epoll_event event; // Add the server socket to the epoll instance
	event.events = EPOLLIN;
	event.data.fd = this->_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_fd, &event) == -1)
		throw std::runtime_error("epoll_ctl() failed");
	std::cout << "Server <" << this->_fd << "> listening on port " << port << "\n";
}

void Server::Monitor() {
	struct epoll_event events[42];

	int event_count = epoll_wait(this->epoll_fd, events, MAX_EVENTS, -1); // check values
	if (event_count == -1)
        return (handleEpollWaitError());

	for (int i = 0; i < event_count; i++)
	{
        if (events[i].events & EPOLLIN)
            handleEpollIn(events[i]);
        else if (events[i].events & (EPOLLERR | EPOLLHUP))
            handleEpollError(events[i]);
    }
}	

void Server::handleEpollWaitError() {
    if (isLive())
        throw std::runtime_error("epoll_wait() failed");
}

void Server::handleEpollIn(const epoll_event &event) {
	if (event.data.fd == this->_fd)
		AcceptNewClient();
    else
		handleClientData(event);
}

void Server::handleClientData(const epoll_event &event) {
    int fd = event.data.fd;
    for (size_t j = 0; j < clientsTable.size(); ++j)
	{
        if (clientsTable[j].getFd() == fd) {
            clientsTable[j].ParseDataClient(fd);
            break;
        }
    }
}

void Server::handleEpollError(const epoll_event &event) {
	std::cerr << "Epoll error on fd: " << event.data.fd << std::endl;
	// unsubscribe(((MainSocket *)events[i].data.ptr)->getFd());
    RemoveClient(event.data.fd);
}

void Server::AcceptNewClient()
{
	Client clientObject; // Create a new client object
	std::memset(&cliadd, 0, sizeof(cliadd)); // Initialize client address structure to zero
	socklen_t len = sizeof(cliadd); // Store the size of the client address structure

	// accept() extracts the 1st connection request from the pending queue, creating a new socket
	int fdClient = accept(_fd, (sockaddr *)&(cliadd), &len);
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
    clientObject.set_IpAdd(inet_ntoa((cliadd.sin_addr))); // Convert the client's IP address from binary to string format and store it
    clientsTable.push_back(clientObject); // Add the new client to the list of connected clients
	std::cout << "Client <" << fdClient << "> Connected" << std::endl;
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

// void Server::RemoveFds(int fd)
// {
// 	// for (size_t i = 0; i < this->fdTable.size(); i++){
// 	// 	if (this->fdTable[i].fd == fd)
// 	// 		{
// 	// 			this->fdTable.erase(this->fdTable.begin() + i);
// 	// 			return;
// 	// 		}
// 	// }
// 	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
//         std::cerr << "epoll_ctl() failed to remove fd: " << fd << std::endl;
//     }
// }
