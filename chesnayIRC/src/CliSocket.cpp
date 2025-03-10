
#include "CliSocket.hpp"
#include "Factory.hpp"


CliSocket::CliSocket(int fd, std::string ip, int port) : _cmd(this)
{
    this->_fd = fd;
    this->_ip = ip;
    this->_port = port;
    this->_nickname = "";
    this->_usrname = "";
    this->_realname = "";
    this->_auth = false;

}



CliSocket::~CliSocket()
{
    close(this->_fd);
}

void    CliSocket::action(uint32_t events)
{
    char buffer[1024]; //a changer et a checker
    std::string line;
    ssize_t bytes;

    if ((events & EPOLLERR) || (events & EPOLLHUP) || (events & EPOLLRDHUP)) // de cote
        return Server::getInstance().removeClient(this);

    memset(buffer, 0, sizeof(buffer));

    if (!_saved.empty())
    {
        line += _saved;
        _saved.clear();
    }

    bytes = recv(_fd, buffer, 1024, 0);
    line += buffer;

    if (line.empty() || bytes == -1)
        return Server::getInstance().removeClient(this);
    
    
    if (!(this->isInList(Server::getInstance().getFdList()))) // de cote
        return Server::getInstance().removeClient(this);

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
            _saved = lineRead;
            return;
        }
        if (!lineRead.empty())
        {
            std::cout << "CLIENT " << this->getFd() << ">> " << lineRead;
            if (this->isInList(Server::getInstance().getFdList()))
                _cmd.parse(lineRead);
        }
    } while (pos != std::string::npos);

} 


bool CliSocket::isInList(std::set<int> list)
{
    return list.find(this->getFd()) != list.end();
}

bool CliSocket::isInList(std::set<std::string> list)
{
    return list.find(this->getSource()) != list.end();
}


// GETTERS

std::string CliSocket::getNick()
{
    return this->_nickname;
}

std::string CliSocket::getUsrName()
{
    return this->_usrname;
}

std::string CliSocket::getRealName()
{
    return this->_realname;
}

std::string CliSocket::getSource()
{
    std::string source = ":" + _nickname + "!" + _usrname + "@" + _ip;
    return source;
}







// SETTERS

void CliSocket::setAuth()
{
    this->_auth = true;
}

bool CliSocket::isAuth()
{
    return this->_auth;
}

void CliSocket::setNick(std::string arg)
{
    this->_nickname = arg;
}

void CliSocket::setUsrName(std::string arg)
{
    this->_usrname = arg;
}

void CliSocket::setRealName(std::string arg)
{
    this->_realname = arg;
}

