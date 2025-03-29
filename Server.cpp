#include "Server.hpp"

Server::Server() : _fd(-1), _serverName("Jean-Claude") { }

Server::~Server()
{
	closeFds();
	close(_fd);

	for (std::vector<Client*>::iterator it = _clientVec.begin(); it != _clientVec.end(); ++it)
		delete (*it);
	_clientVec.clear();

	for (std::set<Channel*>::iterator it = _channelSet.begin(); it != _channelSet.end(); ++it)
		delete (*it);
	_channelSet.clear();

	for (std::map<std::string, Command*>::iterator it = _CommandMap.begin(); it != _CommandMap.end(); ++it)
		delete it->second;
	_CommandMap.clear();
}

// Initialize the server

Server &Server::getInstance()
{
	static Server	instance;
	return (instance);
}

void Server::init()
{
	this->_live = true;
	this->_port = 4444;
	this->_pass = "1234";
	this->_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
	if (this->_fd == -1)
		throw std::runtime_error("socket() failed");

	int opt = 1; // Set socket options
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed");

	sockaddr_in server_addr; // Bind the socket to an address and _port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->_port);
	if (bind(this->_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("bind() failed");
		
	if (listen(this->_fd, SOMAXCONN) == -1) // Listen for incoming connections
		throw std::runtime_error("listen() failed");

	this->_epollFd = epoll_create1(0); // Create an epoll instance
	if (this->_epollFd == -1)
		throw std::runtime_error("epoll_create1() failed");

	struct epoll_event event; // Add the server socket to the epoll instance
	event.events = EPOLLIN;
	event.data.fd = this->_fd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, this->_fd, &event) == -1)
		throw std::runtime_error("epoll_ctl() failed");
	std::cout << "Server <" << this->_fd << "> listening on port " << _port << "\n";
	std::cout << "key is : " << _pass << "\n";
	Reply::initReplies();
	initCommandMap();
}

void Server::init(char **argv)
{
	this->_live = true;
	this->_port = atoi(argv[1]);
	//parser _port
	this->_pass = argv[2];
	//parser _Pass
	this->_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
	if (this->_fd == -1)
		throw std::runtime_error("socket() failed");

	int opt = 1; // Set socket options
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed");

	sockaddr_in server_addr; // Bind the socket to an address and _port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->_port);
	if (bind(this->_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("bind() failed");
		
	if (listen(this->_fd, SOMAXCONN) == -1) // Listen for incoming connections
		throw std::runtime_error("listen() failed");

	this->_epollFd = epoll_create1(0); // Create an epoll instance
	if (this->_epollFd == -1)
		throw std::runtime_error("epoll_create1() failed");

	struct epoll_event event; // Add the server socket to the epoll instance
	event.events = EPOLLIN;
	event.data.fd = this->_fd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, this->_fd, &event) == -1)
		throw std::runtime_error("epoll_ctl() failed");
	std::cout << "Server <" << this->_fd << "> listening on _port " << _port << "\n";
	std::cout << "key is : " << _pass << "\n";
	initCommandMap();
}

void Server::initCommandMap()
{
	_CommandMap["CAP"] = new CommandCap();
	_CommandMap["NICK"] = new CommandNick();
	_CommandMap["USER"] = new CommandUser();
	_CommandMap["MODE"] = new CommandMode();
	_CommandMap["JOIN"] = new CommandJoin();
	_CommandMap["PASS"] = new CommandPass();
	_CommandMap["PING"] = new CommandPing();
	_CommandMap["PRIVMSG"] = new CommandPrivMsg();
	_CommandMap["TOPIC"] = new CommandTopic();
	_CommandMap["PART"] = new CommandPart();
	_CommandMap["INVITE"] = new CommandInvite();
	_CommandMap["KICK"] = new CommandKick();
}

// Closing functions
void Server::shutdown() { _live = false; }

void Server::closeFds()
{
	for(size_t i = 0; i < _clientVec.size(); i++)
	{
		std::cout << "Client <" << _clientVec[i]->get_fd() << "> Disconnected" << std::endl;
		close(_clientVec[i]->get_fd());
	}
	if (_fd != -1)
	{
		std::cout << "Server <" << _fd << "> disconnected" << std::endl;
		close(_fd);
	}
}

// Server getters/setters

const std::string &Server::get_serverName() const { return (_serverName); }
bool Server::is_live () const { return (_live); }
const std::string &Server::get_pass() const { return (_pass); }
const std::map<std::string, Command*> &Server::get_CommandMap() const { return (_CommandMap); }

// Server data handling

void Server::monitor()
{
	struct epoll_event events[42];
	int event_count = epoll_wait(this->_epollFd, events, 10, -1); // check values
	if (event_count == -1)
	{
		if (is_live())
		throw std::runtime_error("epoll_wait() failed");
	}
	for (int i = 0; i < event_count; i++)
	{
		if (events[i].events & EPOLLIN)
		{
			if (events[i].data.fd == this->_fd)
				acceptNewClient();
			else
				handleDataClient(events[i]);			
		}
		else if (events[i].events & (EPOLLERR | EPOLLHUP))
		{
			std::cerr << "Epoll error on fd: " << events[i].data.fd << std::endl;
			// unsubscribe(((MainSocket *)events[i].data.ptr)->get_fd());
			removeClient(events[i].data.fd);
		}
	}
}

void Server::acceptNewClient()
{	
	std::memset(&_cliadd, 0, sizeof(_cliadd)); // Initialize client address structure to zero
	socklen_t len = sizeof(_cliadd); // Store the size of the client address structure
	int fdClient = accept(_fd, (sockaddr *)&(_cliadd), &len); // accept() extracts the 1st connection request from the pending queue, creating a new socket
	if (fdClient == -1)
	{ std::cout << "accept() failed" << std::endl; return; }
	if (fcntl(fdClient, F_SETFL, O_NONBLOCK) == -1) // fcntl() edits the fd and sets the new socket to non-blocking mode
	{ std::cout << "fcntl() failed" << std::endl; return; }
	struct epoll_event event;
	event.events = EPOLLIN; // Set the event type for epoll(). EPOLLIN: The client socket is ready for reading
	event.data.fd = fdClient;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fdClient, &event) == -1) // Add the client socket to the epoll instance
	{
		std::cout << "epoll_ctl() failed" << std::endl;
		close(fdClient);
		return;
	}
	std::string ip = inet_ntoa(_cliadd.sin_addr);
	int _port = ntohs(_cliadd.sin_port);
	Client* clientObject = new Client(fdClient, ip, _port);
	_clientVec.push_back(clientObject); // Add the new client to the list of connected clients
	std::cout << "Client <" << fdClient << "> Connected" << std::endl;
}

void Server::handleDataClient(const epoll_event &event)
{
	int fd = event.data.fd;
	for (size_t j = 0; j < _clientVec.size(); ++j)
	{
		if (_clientVec[j]->get_fd() == fd) {
			_clientVec[j]->parseDataClient();
			break;
		}
	}
}

void Server::addChannel(Client *client, std::string name, std::string key)
{
	Channel *channel = new Channel(client, name, key);
	_channelSet.insert(channel);
	channel->set_founderMask(client->get_mask());
	channel->joinChannel(client, key);
}

void Server::removeClient(int fd)
{
	for (size_t i = 0; i < this->_clientVec.size(); i++)
	{
		if (this->_clientVec[i]->get_fd() == fd)
		{
			this->_clientVec.erase(this->_clientVec.begin() + i);
			return;
		}
	}
}

// Clients and channels setters/getters

bool	Server::isClient(const std::string &nickname) const
{
	for (size_t i = 0; i < _clientVec.size(); ++i)
		if (_clientVec[i]->get_nickname() == nickname)
			return (true);
	return (false);
}

bool	Server::isRegisteredClient(const std::string &nickname) const
{
	for (size_t i = 0; i < _clientVec.size(); ++i)
		if (_clientVec[i]->get_nickname() == nickname && _clientVec[i]->get_isRegistered())
			return (true);
	return (false);
}

Client *Server::get_client(const std::string &nickname)
{
	for (size_t i = 0; i < _clientVec.size(); ++i)
		if (_clientVec[i]->get_nickname() == nickname)
			return (_clientVec[i]);
	return (NULL);
}

const std::vector<Client*>	&Server::get_clientVec() const { return _clientVec; }
const std::set<Channel*>	&Server::get_channelSet() const { return _channelSet; }
