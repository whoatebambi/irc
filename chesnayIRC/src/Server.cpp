
#include "Server.hpp"
#include "MainSocket.hpp"
#include "SerSocket.hpp"
#include "CliSocket.hpp"
#include "Channel.hpp"

//----TO BE MOVED TO .CPP----

Server::Server() {}

Server::~Server()
{

    // Delete clients
    for (std::vector<CliSocket*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        unsubscribe((*it)->getFd());
        delete *it;
    }
    _clients.clear();

    // Delete Chans
    for (std::map<std::string, Channel*>::iterator it = _channelMap.begin(); it != _channelMap.end(); ++it)
    {
        delete it->second;
    }
    _channelMap.clear();

    close(this->_epfd);
}

// PARSE FUNCTIONS COULD BE MOVED INTO MAIN TO CLEAN THE CLASS AND VARY ;(put port and pass into init(port, pass)))

std::string parsePort(const std::string port)
{
    for (size_t i = 0; i < port.length(); ++i)
    {
        if (!isdigit(port[i]))
            throw std::invalid_argument("Port must be a valid number.");
    }

    int intPort = strtol(port.c_str(), NULL, 10);

    if (intPort < 1024 || intPort > 49151)
        throw std::out_of_range("Port must be between 1024 and 49151");
    return port;
}

std::string parsePwd (const std::string pwd)
{
    if (pwd.length() < 4 || pwd.length() > 10)
        throw std::invalid_argument("Password size issue. Must be between 4 and 10 characters long.");
    for (size_t i = 0; i < pwd.length(); ++i)
    {
        if (!isalnum(pwd[i]))
            throw std::invalid_argument("Password must contain only alphanum characters.");
    }
    return pwd;
}


void Server::init(char **av)
{
    this->_port = parsePort(av[1]);
    this->_password = parsePwd(av[2]);
    this->_serSocket.init(this->_port.c_str()); // PQ PAS FAIRE TOUT SUR UNE LIGNE COMME DES PRO GAMERS ??
    this->_epfd = epoll_create1(0); // DO NOT FORGET TO CLOSE EPFD LATER
    if (this->_epfd < 0)
        throw ServExcept("Epoll Create failed"); // throw
    this->_isLive = true;
    // other shits may be added later
}

void Server::subscribe(MainSocket &socket, int fd)
{
    struct epoll_event event;

    memset(&event, 0, sizeof event);

    event.events = EPOLLIN | EPOLLRDHUP;
    event.data.ptr = &socket;

    if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &event))
        throw ServExcept("Epoll Control failed");

}

void Server::boot()
{
    int optval = 1;

    setsockopt(this->_serSocket.getFd(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)); // ALLOW REUSE OF PORT

    if (bind(this->_serSocket.getFd(), this->_serSocket.getAddr()->ai_addr, this->_serSocket.getAddr()->ai_addrlen)) // BIND THE FD TO A PORT AND IP
        throw ServExcept("Bind on server socket failed");
    
    if (listen(this->_serSocket.getFd(), SOMAXCONN)) // PORT IS SET AS "LISTENING"
        throw ServExcept("Listen on server socket failed");
    
    if (fcntl(this->_serSocket.getFd(), F_SETFL, O_NONBLOCK)) // ENABLES NON-BLOCKING STUFF 
        throw ServExcept("Fcntl on server socket failed");
    
    subscribe(this->_serSocket, this->_serSocket.getFd()); 
    
    // INSERER ICI PEUT ETRE REGISTERED MAIS PAS SUR DE L'UTILITE

    std::cout << "SERVER IS READY TO PROUT" << std::endl;

}

void Server::monitor()
{
    struct epoll_event events[42];

    int numEvents = epoll_wait(this->_epfd, events, 42, 50); // NOT SURE ABOUT VALUES NEED TO CHECK LATER AND DEFINE

    if (numEvents < 0) // PLACEHOLDER OBVIOUSLY
    {
        if (isLive())
            throw ServExcept("Epoll wait failed");
        else
            return;
    }

    // VERY VERY IMPORTANT PLEASE SER PAY ATTENTION

    for (int i = 0; i < numEvents; i++)
    {
        ((MainSocket *)events[i].data.ptr)->action(events[i].events);
    }

}


void Server::newClient(sockaddr_in client_addr, int newFd)
{
    std::string ip = inet_ntoa(client_addr.sin_addr);
    int port = ntohs(client_addr.sin_port);

    CliSocket* nc = new CliSocket(newFd, ip, port);

    _clients.push_back(nc);
    _clientsFd.insert(nc->getFd());

    subscribe(*nc, newFd);

    std::cout << "Connection detected: " << ip << " : " << port << std::endl; 
}


// Chan interactions functions


void Server::newChan(CliSocket *client, std::string chanName, std::string key)
{
    Channel *newChan = new Channel(client, chanName, key);
    _channelMap.insert(std::make_pair(chanName, newChan));
    newChan->getFounderMask() = client->getSource();
    newChan->joinChan(client, chanName, key);
    //_channelMap[chanName] = newChan;
}

void Server::unsubscribe(int fd)
{
	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw ServExcept("Epoll Control in unsubscribe failed");
}

void Server::removeClient(CliSocket *client)
{
    for (std::map<std::string, Channel*>::iterator it = this->_channelMap.begin(); it != _channelMap.end(); )
    {
        if (client->isInList(it->second->getMembers()))
        {
            Channel::removeFromList(it->second->getMembers(), client->getFd());

            if (it->second->isEmpty()) // delete channel if it is empty
            {
				delete (it->second);
				std::map<std::string, Channel*>::iterator temp = it;
				++it;
				_channelMap.erase(temp);
				continue ;
            }
        }
		++it;
    }
	
	unsubscribe(client->getFd());
    removeFromFdList(client->getFd());
    
    for (std::vector<CliSocket *>::iterator it = this->_clients.begin(); it != this->_clients.end(); )
    {
        if (client == *it)
        {
            delete (*it);
            it = this->_clients.erase(it);
        }
        else
            ++it;
    }
}

bool Server::isLive()
{
    if(this->_isLive)
        return true;
    return false;
}


void Server::removeFromFdList(int fd)
{
    this->_clientsFd.erase(fd);
}








////////////////////////GETTERS


Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

std::string Server::getPwd()
{
    return this->_password;
}

const std::map<std::string, Channel*>& Server::getOniChanUwU()
{
    return this->_channelMap;
}

// gets all fds of clients that share a chan with the client passed as parameter
std::set<int> Server::getSharedChans(CliSocket *client)
{
    std::set<int> list;
    for (std::map<std::string, Channel*>::const_iterator it = this->_channelMap.begin(); it != _channelMap.end(); ++it)
    {
        if (client->isInList(it->second->getMembers()))
        {
            const std::set<int> &members = it->second->getMembers();
            list.insert(members.begin(), members.end());
        }
    }
	if (this->_channelMap.empty() || list.empty())
		list.insert(client->getFd());
    return list;
}

std::vector<CliSocket *> Server::getClients()
{
    return this->_clients;
}

std::set<int> Server::getFdList()
{
    return this->_clientsFd;
}












bool Server::nickTaken(std::string nick)
{
    for (std::vector<CliSocket*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getNick() == nick)
            return true; // taken
    }
    return false; // available
}


// utils

std::string Lower(const std::string &str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), tolower);
    return lowerStr;
}

bool    sameString(const std::string s1, const std::string s2)
{
    std::string ls1 = Lower(s1);
    std::string ls2 = Lower(s2);

    if (ls1 == ls2)
        return true;
    return false;
}



//------------ DEBUG ------------

void Server::printClients(const std::vector<CliSocket*>& clients)
{
	for (std::vector<CliSocket*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
        if (*it)
            std::cout << (*it)->getSource() << std::endl;
    }
}

void Server::printChannels(const std::map<std::string, Channel*>& channelMap)
{
	for (std::map<std::string, Channel*>::const_iterator it = channelMap.begin(); it != channelMap.end(); ++it) 
	{
        if (it->second)
            std::cout << "Channel Key: " << it->first << std::endl;
    }
}

void Server::printSet(const std::set<int>& list) 
{
    for (std::set<int>::const_iterator it = list.begin(); it != list.end(); ++it) 
    {
        std::cout << "PrintSet: " << *it << std::endl;
    }
}



void searchAndReplace(std::set<std::string> &strSet, const std::string &oldmask, const std::string &newmask)
{
    if (sameString(oldmask, newmask))
        return;

    std::set<std::string> updatedSet;

    for (std::set<std::string>::iterator it = strSet.begin(); it != strSet.end(); ++it)
    {
        if (sameString(*it, oldmask))
        {
            updatedSet.insert(newmask);
            std::cout << "=====================================S AND R  1st condition: " << oldmask << " with " << newmask << " in updated set " << *it << std::endl;
        }
        else
        {
            std::cout << "=====================================S AND R  2nd condition: " << oldmask << " with " << newmask << " in updated set " << *it << std::endl;
            updatedSet.insert(*it);
        }
    }
    strSet.swap(updatedSet);
}


void printBan(const std::set<std::string>& list)
{
    for (std::set<std::string>::const_iterator it = list.begin(); it != list.end(); ++it) 
    {
        std::cout << "BanList>>>>>>>>>>>>>: " << *it << std::endl;
    }
}
void Server::updateList(const std::string &oldmask, const std::string &newmask)
{
    for(std::map<std::string, Channel *>::iterator it = _channelMap.begin(); it != _channelMap.end(); ++it)
    {
        printBan(it->second->getBanList());
        searchAndReplace(it->second->getBanList(), oldmask, newmask);
        std::cout << "=====================================Should have replaced " << oldmask << " with " << newmask << " in " << it->second->getName() << std::endl;
        printBan(it->second->getBanList());
        searchAndReplace(it->second->getWhiteList(), oldmask, newmask);
        searchAndReplace(it->second->getOpsList(), oldmask, newmask);
    }
}

void Server::shutdown()
{
    this->_isLive = false;
}