/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/10 15:05:21 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <set>
#include <sstream>
#include <map>
#include <functional>


#include "CapCommand.hpp"


Client::Client()
{
	this->_nickname = "";
	this->_username = "";
	this->_ipadd = "";
	this->_fd = -1;

    this->_commandMap["CAP"] = new CapCommand();
}
Client::Client(std::string _nickname, std::string _username, int _fd) :_fd(_fd), _nickname(_nickname), _username(_username){}
Client::~Client(){
    for (std::map<std::string, Command*>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it) {
        delete it->second;
    }
    _commandMap.clear();
}
Client::Client(Client const &src){*this = src;}
Client &Client::operator=(Client const &src){
	if (this != &src){
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_ipadd = src._ipadd;
		this->_fd = src._fd;
	}
	return (*this);
}

void Client::SetFd(int _fd){this->_fd = _fd;}
void Client::set_IpAdd(std::string _ipadd){this->_ipadd = _ipadd;}

int Client::getFd() const {return this->_fd;}
std::string Client::getNickname() const {return this->_nickname;}
std::string Client::getSaved() const {return this->_saved;}


void	Client::ParseDataClient(int fd) {	 // swap fd for (uint32_t events)
	std::string line;

    if (!this->_saved.empty())
    {
		line += this->_saved;
        this->_saved.clear();
    }

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);
	line += buffer;
	if (line.empty() || bytes == -1)
	{
		return Server::getInstance().RemoveClient(fd); // swap fd for (CliSocket *client)
		// RemoveClient(fd);
		// RemoveFds(fd);
		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
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
			parse(lineRead); // + check if in list
    } while (pos != std::string::npos);
}


void Client::parse(std::string &line)
{
	std::cout << "<<< " << line;
    size_t pos = line.find(' ');
    std::string args;
    std::string cmd = line.substr(0, pos);
    if (pos != std::string::npos) 
        args = ft_trim(line.substr(pos + 1));

    std::map<std::string, Command*>::iterator it = _commandMap.find(cmd);
    if (it != _commandMap.end())
        it->second->execute(args, this);
	else if (cmd == "NICK")
        NickCommand(args);
    else if (cmd == "USER")
        UserCommand(args);
    else
        std::cout << "Unknown command: " << cmd << std::endl;

    std::string msg = _ipadd + " " + _nickname + " " + _username + "\r\n";
    //std::cout << std::endl << msg << std::endl << std::endl;
    send(this->_fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
}


std::string Client::ft_trim(const std::string &str)
{
    size_t end = str.find_last_not_of("\r\n");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}


void Client::NickCommand(const std::string &args)
{   
    std::string arr = "[]{}\\|";

    std::string oldnick = this->_nickname;
	(void)args;
    
    std::string msg = "<<<< NICKKKKKKKK ";
	msg += "\r\n";
    send(this->_fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    _nickname = "Warwick";
}

std::vector<std::string> splitArgs(const std::string &input)
{
    std::vector<std::string> vec;
    std::istringstream iss(input);
    std::string word;
    //bool trailing = false;

    while(iss >> word)
    {
        if (word[0] == ':')
        {
            //trailing = true;
            vec.push_back(input.substr(input.find(':') + 1));
            break;
        }
        vec.push_back(word);
    }
    return vec;
    // (void)trailing;
}

void sendRpl(int fd, std::string err, ...)
{
    va_list args;
    va_start(args, err);
    
    std::string msg = "<<<<<< USERRERERERER";
    msg += "\r\n";

    send(fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    va_end(args);
}

void Client::UserCommand(const std::string &args)
{
    std::vector<std::string> arg = splitArgs(args);

    //std::cout << "USER COMMAND EXECUTE DEBUG" << std::endl;

    if (arg.size() < 4)
        return sendRpl(this->_fd, "ERR_NEEDMOREPARAMS", this->_nickname.c_str());

    if (!(this->_username.empty()))
        return sendRpl(this->_fd, "ERR_ALREADYREGISTERED", this->_nickname.c_str());

    std::string msg = "<<<<<< USERRERERERER";
    msg += "\r\n";
    send(this->_fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    this->_username = "FURRY <3";
    //this->_realName = arg[3];
    //sendWelcome(client);
}
