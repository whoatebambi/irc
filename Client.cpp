#include "Client.hpp"

Client::Client(int fd, std::string ip, int port)
	: _fd(fd),
	_ipadd(ip),
	_port(port)
{
	this->_isAuth = false;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";

	this->_CommandMap["CAP"] = new CommandCap();
	this->_CommandMap["NICK"] = new CommandNick();
	this->_CommandMap["USER"] = new CommandUser();
	this->_CommandMap["MODE"] = new CommandMode();
	this->_CommandMap["JOIN"] = new CommandJoin();
	this->_CommandMap["PASS"] = new CommandPass();
	this->_CommandMap["PING"] = new CommandPing();
	this->_CommandMap["PRIVMSG"] = new CommandPrivMsg();
	this->_CommandMap["TOPIC"] = new CommandTopic();
	this->_CommandMap["PART"] = new CommandPart();
	this->_CommandMap["INVITE"] = new CommandInvite();
}

Client::~Client()
{
	// std::cout << "Client <" << this->_fd << "> destructor called" << std::endl;

	for (std::map<std::string, Command*>::iterator it = this->_CommandMap.begin(); it != this->_CommandMap.end(); ++it) {
		delete it->second;
	}
	this->_CommandMap.clear();
}

void	Client::parseDataClient()
{
	std::string line;

	if (!this->_saved.empty())
	{
		line += this->_saved;
		this->_saved.clear();
	}

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(this->_fd, buffer, sizeof(buffer) - 1 , 0);
	line += buffer;
	if (line.empty() || bytes == -1)
	{
		return Server::getInstance().RemoveClient(this->_fd); // swap fd for (Client *client)
		// RemoveClient(fd);
		// RemoveFds(fd);
		std::cout << "Client <" << this->_fd << "> Disconnected" << std::endl;
		// close(fd);
	}
	
	// check if in list
	size_t pos;
	std::string lineRead;
	do
	{
		pos = line.find("\r\n");
		if (pos != std::string::npos)
			pos += 2;
		lineRead = line.substr(0, pos);
		line = line.erase(0, pos);
		if (pos == std::string::npos && !lineRead.empty())
		{
			this->_saved = lineRead;
			return;
		}
		if (!lineRead.empty())
			executeCommand(lineRead);
	} while (pos != std::string::npos);
}

void Client::executeCommand(std::string &line)
{
	std::cout << "<<< " << line;
	size_t pos = line.find(' ');
	std::string args;
	std::string cmd = line.substr(0, pos);
	if (pos != std::string::npos) 
		{args = ft_trim(line.substr(pos + 1));}
	if (this->_CommandMap.find(cmd) != this->_CommandMap.end())
		this->_CommandMap[cmd]->execute(args, this);
	else
		std::cout << "Unknown command: " << cmd << std::endl;
}

std::string Client::ft_trim(const std::string &str)
{
	size_t end = str.find_last_not_of("\r\n");
	return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

// remove if possible
bool Client::isInList(const std::set<Client*> &list) const { return list.find(const_cast<Client*>(this)) != list.end();; }

std::string	Client::get_nickname() const {return _nickname;}
void		Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
std::string	Client::get_mask() const { return _mask; }
void		Client::set_mask() { _mask = ":" + _nickname + "!" + _username + "@" + _ipadd; }
bool		Client::get_isAuth() const {return this->_isAuth;}
void		Client::set_isAuth() {this->_isAuth = true;}
bool		Client::get_isRegistered() const {return this->_isRegistered;}
void		Client::set_isRegistered() {this->_isRegistered = true;}
int			Client::get_fd() const {return this->_fd;}
std::string	Client::get_saved() const {return this->_saved; }
std::string	Client::get_username() const { return this->_username; }
void		Client::set_username(const std::string &username) { this->_username = username; }
std::string	Client::get_realname() const { return this->_realname; }
void		Client::set_realname(const std::string &realname) { this->_realname = realname; }
