#include "Client.hpp"

Client::Client(int fd, std::string ip, int port)
{
	this->_fd = fd;
	this->_ipadd = ip;
	this->_port = port;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";

	this->_CommandMap["CAP"] = new CommandCap();
	this->_CommandMap["NICK"] = new CommandNick();
	this->_CommandMap["USER"] = new CommandUser();
	this->_CommandMap["MODE"] = new CommandMode();
}

Client::~Client()
{
	// std::cout << "Client <" << this->_fd << "> destructor called" << std::endl;

	for (std::map<std::string, Command*>::iterator it = this->_CommandMap.begin(); it != this->_CommandMap.end(); ++it) {
		delete it->second;
	}
	this->_CommandMap.clear();
}

int	Client::getFd() const {return this->_fd;}
std::string	Client::getNickname() {return this->_nickname;}
void Client::setNickname(const std::string &nickname) {this->_nickname = nickname;}
std::string	Client::getSaved() const {return this->_saved; }
std::string Client::getUsername() const { return this->_username; }
void Client::setUsername(const std::string &username) { this->_username = username; }
std::string Client::getRealname() const { return this->_realname; }
void Client::setRealname(const std::string &realname) { this->_realname = realname; }


void	Client::ParseDataClient()
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
		return Server::getInstance().RemoveClient(this->_fd); // swap fd for (CliSocket *client)
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
			parse(lineRead);
	} while (pos != std::string::npos);
}

void Client::parse(std::string &line)
{
	std::cout << "<<< " << line;
	size_t pos = line.find(' ');
	std::string args;
	std::string cmd = line.substr(0, pos);
	if (pos != std::string::npos) 
		{args = ft_trim(line.substr(pos + 1));}

	if (cmd == "CAP")
		this->_CommandMap["CAP"]->execute(args, this);
	else if (cmd == "NICK")
		this->_CommandMap["NICK"]->execute(args, this);
	else if (cmd == "USER")
		this->_CommandMap["USER"]->execute(args, this);
	else if (cmd == "MODE")
		this->_CommandMap["MODE"]->execute(args, this);
	else if (cmd == "PING")
	{
		std::cout << "RECEIVED PNG MESSAGGEEEEEEEEEE" << line;
		std::string msg = "PONG " + args + "\r\n";
    	send(this->_fd, (msg).c_str(), msg.size(), MSG_NOSIGNAL);	
	}
	else
		std::cout << "Unknown command: " << cmd << std::endl;
}

std::string Client::ft_trim(const std::string &str)
{
	size_t end = str.find_last_not_of("\r\n");
	return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string Client::getSource()
{
    std::string source = ":" + _nickname + "!" + _username + "@" + _ipadd;
    return (source);
}
