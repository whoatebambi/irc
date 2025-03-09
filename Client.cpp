/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/09 17:20:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->nickname = "";
	this->username = "";
	this->ipadd = "";
	this->_fd = -1;
}
Client::Client(std::string nickname, std::string username, int _fd) :_fd(_fd), nickname(nickname), username(username){}
Client::~Client(){}
Client::Client(Client const &src){*this = src;}
Client &Client::operator=(Client const &src){
	if (this != &src){
		this->nickname = src.nickname;
		this->username = src.username;
		this->ipadd = src.ipadd;
		this->_fd = src._fd;
	}
	return (*this);
}

void Client::SetFd(int _fd){this->_fd = _fd;}
void Client::setIpAdd(std::string ipadd){this->ipadd = ipadd;}

int Client::getFd() const {return this->_fd;}
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
		// std::cout << "parse()" << std::endl;
    } while (pos != std::string::npos);
}


void Client::parse(std::string &line)
{
	std::cout << "DEBUG_CLIENT_DATA = " << line << std::endl;
    size_t pos = line.find(' ');
    std::string args;
    std::string cmd = line.substr(0, pos);
    if (pos != std::string::npos) 
        args = ft_trim(line.substr(pos + 1));
    else 
        args = "";

    if (cmd == "CAP")
		sendMsg("CAP * LS :");
	else if (cmd == "JOIN")
		sendMsg( "JOINNNNNN");
    else
        std::cout << "Unknown command: " << cmd << std::endl;
}


std::string Client::ft_trim(const std::string &str)
{
    size_t end = str.find_last_not_of("\r\n");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}


void Client::sendMsg(const std::string &msg) {
	std::cout << "server.sendMsg() = " << msg << std::endl;
    send(this->getFd(), msg.c_str(), msg.length(), 0);
}