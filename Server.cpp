#include "Server.hpp"

Server::Server() : _fd(-1), _host("Jean-Claude") { }

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

	for (std::map<std::string, Command*>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it)
		delete it->second;
	_commandMap.clear();
	delete _poller;
}

// Initialize the server

Server &Server::getInstance()
{
	static Server	instance;
	return (instance);
}

void Server::init(std::string port, std::string password)
{
	_running = true;
	_port = atoi(port.c_str());
	_password = password;

	createServerSocket();
	bindAndListen();
	setupPoller();
	addServerSocketToPoller();
	initCommandMap();

	std::cout << "Server <" << _fd << "> listening on _port " << _port << std::endl;
	std::cout << "key is : " << _password << std::endl;
}

void Server::createServerSocket()
{
	_fd = socket(AF_INET, SOCK_STREAM, 0); // Create socket
	if (_fd == -1)
		throw std::runtime_error("socket() failed");

	int opt = 1; // Set socket options
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed");
}

void Server::bindAndListen()
{
	sockaddr_in server_addr; // Bind the socket to an address and _port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	if (bind(_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("bind() failed");

	if (listen(_fd, SOMAXCONN) == -1) // Listen for incoming connections
		throw std::runtime_error("listen() failed");
}

void Server::setupPoller()
{
	try {
		_poller = createPoller();
	} catch (const std::exception& e) {
		throw std::runtime_error(std::string("Poller setup failed: ") + e.what());
	}
}

void Server::addServerSocketToPoller()
{
	if (!_poller->add(_fd))
		throw std::runtime_error("Poller::add() failed for server socket");
}

void Server::initCommandMap()
{
	_commandMap["CAP"] = new CommandCap();
	_commandMap["NICK"] = new CommandNick();
	_commandMap["USER"] = new CommandUser();
	_commandMap["MODE"] = new CommandMode();
	_commandMap["JOIN"] = new CommandJoin();
	_commandMap["PASS"] = new CommandPass();
	_commandMap["PING"] = new CommandPing();
	_commandMap["PRIVMSG"] = new CommandPrivMsg();
	_commandMap["TOPIC"] = new CommandTopic();
	_commandMap["PART"] = new CommandPart();
	_commandMap["INVITE"] = new CommandInvite();
	_commandMap["KICK"] = new CommandKick();
}

// Closing functions
void Server::shutdown() { _running = false; }

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

const std::string &Server::get_host() const { return (_host); }
bool Server::is_running () const { return (_running); }
const std::string &Server::get_password() const { return (_password); }
const std::map<std::string, Command*> &Server::get_CommandMap() const { return (_commandMap); }

// Server data handling

void Server::monitor()
{
	PollEvent events[42];
	int eventCount = _poller->wait(events, 42, -1);

	if (eventCount == -1 && is_running())
		throw std::runtime_error("Poller::wait() failed");

	for (int i = 0; i < eventCount; ++i)
		handlePollEvent(events[i]);
}

void Server::handlePollEvent(const PollEvent& event)
{
	if (event.readable)
	{
		if (event.fd == _fd)
			acceptNewClient();
		else
			handleDataClient(event.fd);
	}
	else if (event.error)
	{
		std::cerr << "Poll error on fd: " << event.fd << std::endl;
		removeClient(event.fd);
	}
}

void Server::acceptNewClient()
{	
	int clientFd = acceptSocketClient();
	if (clientFd == -1)
		return;

	// fcntl() edits the fd and sets the new socket to non-blocking mode
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		close(clientFd);
		return;
	}

	if (!_poller->add(clientFd)) // add client fd to Poller
	{
		close(clientFd);
		return;
	}

	createAndStoreClient(clientFd);
}

int Server::acceptSocketClient()
{
	std::memset(&_cliadd, 0, sizeof(_cliadd)); // Initialize client address structure to zero
	socklen_t len = sizeof(_cliadd); // Store the size of the client address structure
	int clientFd = accept(_fd, (sockaddr*)&_cliadd, &len); 
	// accept() extracts the 1st connection request from the pending queue, creating a new socket
	if (clientFd == -1)
		std::cout << "accept() failed" << std::endl;
	return clientFd;
}

void Server::createAndStoreClient(int clientFd)
{
	std::string ip = inet_ntoa(_cliadd.sin_addr);
	int port = ntohs(_cliadd.sin_port);
	Client* clientObject = new Client(clientFd, ip, port);
	_clientVec.push_back(clientObject);
	std::cout << "Client <" << clientFd << "> Connected" << std::endl;
}

void Server::handleDataClient(int fd)
{
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
			delete _clientVec[i];
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
