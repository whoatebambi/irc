#include "Client.hpp"

Client::Client(int fd, std::string host, int port) : _fd(fd), _host(host), _port(port), _isAuth(false) {}

Client::~Client() {}

void	Client::parseDataClient()
{
	std::string line;

	if (!_saved.empty())
	{
		line += _saved;
		_saved.clear();
	}

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(_fd, buffer, sizeof(buffer) - 1 , 0);
	line += buffer;
	if (line.empty() || bytes == -1)
	{
		std::cout << "Client <" << _fd << "> Disconnected" << std::endl;
		Server::getInstance().removeClient(_fd);
		return;
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
			_saved = lineRead;
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

	const std::map<std::string, Command*> &_commandMap = Server::getInstance().get_CommandMap();
	std::map<std::string, Command*>::const_iterator it = _commandMap.find(cmd);
	if (it != _commandMap.end())
		it->second->execute(args, this);
	else
		std::cout << "Unknown command: " << cmd << std::endl;
}

std::string Client::ft_trim(const std::string &str)
{
	size_t end = str.find_last_not_of("\r\n");
	return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

// remove if possible

const std::string	&Client::get_host() const { return _host; };
const std::string	&Client::get_nickname() const { return _nickname; }
void				Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
const std::string	&Client::get_mask() const { return _mask; }
void				Client::set_mask() { _mask = ":" + _nickname + "!" + _username + "@" + _host; }
bool		Client::get_isAuth() const { return _isAuth; }
void		Client::set_isAuth() { _isAuth = true;}
bool		Client::get_isRegistered() const { return _isRegistered; }
void		Client::set_isRegistered() { _isRegistered = true; }
void		Client::set_isDead() { _isDead = true; }
bool		Client::get_isDead() const { return _isDead; }
int			Client::get_fd() const { return _fd; }
const std::string	&Client::get_username() const { return _username; }
void				Client::set_username(const std::string &username) { _username = username; }
const std::string	&Client::get_realname() const { return _realname; }
void				Client::set_realname(const std::string &realname) { _realname = realname; }
std::string Client::get_source() const {
	
	std::cout << "SOURCE1 hostname :" << _host << std::endl;
	std::string source = ":" + _nickname + "!" + _username + "@" + _host;
	std::cout << "SOURCE2" << source << std::endl;
	return source;
}