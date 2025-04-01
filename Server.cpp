#include "Server.hpp"

Server::Server() : _fd(-1), _serverName("Jean-Claude") { }

Server::~Server()
{
	closeFds();
	close(_fd);

	for (size_t i = 0; i < _clientVec.size(); ++i)
	{
		// removeClient(_clientVec[i]->get_fd());
    	delete _clientVec[i];
	}
	_clientVec.clear();

	std::vector<Channel*> channelsToDelete(_channelSet.begin(), _channelSet.end());
	for (size_t i = 0; i < channelsToDelete.size(); ++i)
		removeChannel(channelsToDelete[i]);
	_channelSet.clear(); // not strictly needed, but safe to keep

	for (std::map<std::string, Command*>::iterator it = _commandMap.begin(); it != _commandMap.end(); )
	{
		delete (it->second);
		_commandMap.erase(it++); // increment *after* erasing
	}
	_commandMap.clear();
	delete _poller;
}

// Initialize the server

Server &Server::getInstance()
{
	static Server	instance;
	return (instance);
}

void Server::init(const std::string &port, const std::string &password)
{
	_running = true;
	_port = atoi(port.c_str());
	if (_port < MIN_PORT || _port > MAX_PORT)
		throw std::runtime_error("Invalid port: must be between 1024 and 65535");
	_password = password;

	createServerSocket();
	bindAndListen();
	setupPoller();
	addServerSocketToPoller();
	Reply::initReplies();
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
	_commandMap["QUIT"] = new CommandQuit();
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

const std::string &Server::get_serverName() const { return (_serverName); }
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
	std::string host = inet_ntoa(_cliadd.sin_addr);
	int port = ntohs(_cliadd.sin_port);
	Client* clientObject = new Client(clientFd, host, port);
	_clientVec.push_back(clientObject);
	std::cout << "Client <" << clientFd << "> Connected" << std::endl;
}

void Server::handleDataClient(int fd)
{
	for (size_t j = 0; j < _clientVec.size(); ++j)
	{
		// if (_clientVec[j] == NULL)
		// 	continue;
		if (_clientVec[j]->get_fd() == fd)
		{
			_clientVec[j]->parseDataClient();
			// if (_clientVec[j]->get_isDead() == true)
			// 	std::cout << "_clientVec[j]->get_isDead() == true" << std::endl;
			// 	Server::getInstance().removeClient(_clientVec[j]->get_fd());
			break;
		}
	}
}

void Server::addChannel(Client *client, const std::string &name, const std::string &key)
{
	Channel *channel = new Channel(client, name, key);
	_channelSet.insert(channel);
	channel->set_founderMask(client->get_mask());
	channel->joinChannel(client, key);
}

void Server::removeChannel(Channel *channel)
{
	std::cout << "Channel #" << channel->get_name() << " deleted.\n";
	_channelSet.erase(channel); // remove pointer from Server's set
	delete (channel); // this will destroy the channel but not the clients in the lists)
}

void Server::removeClient(int fd)
{
	for (size_t i = 0; i < this->_clientVec.size(); ++i)
	{
		Client* client = this->_clientVec[i];
		if (client->get_fd() == fd)
		{
			for (std::set<Channel*>::iterator it = _channelSet.begin(); it != _channelSet.end(); ++it)
			{
				Channel* channel = *it;
				if ((*it) == NULL)
					continue;
				channel->removeMember(client);
				channel->removeInvite(client);
				channel->removeOperator(client);
				// if (channel->get_memberSet().size() == 0)
				// 	removeChannel(channel);
			}
			std::cout << "Client <" << client->get_fd() << ">" << " deleted.\n";
			_poller->unregisterFd(fd);
			close(fd);
			delete (client);
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

std::set<int> Server::getSharedMembersFd(Client *client)
{
    std::set<int> list;
    for (std::set<Channel *>::const_iterator it = this->_channelSet.begin(); it != _channelSet.end(); ++it)
    {
		if ((*it)->isInChannel(client))
        {
			std::set<int> memberFdSet;
			for (std::set<Client*>::iterator it2 = (*it)->get_memberSet().begin(); it2 != (*it)->get_memberSet().end(); ++it2)
				if ((*it2)->get_fd() != client->get_fd())
					memberFdSet.insert((*it2)->get_fd());
            list.insert(memberFdSet.begin(), memberFdSet.end());
        }
    }
	// list.insert(client->get_fd());
    return list;
}

const std::vector<Client*>	&Server::get_clientVec() const { return _clientVec; }
const std::set<Channel*>	&Server::get_channelSet() const { return _channelSet; }
