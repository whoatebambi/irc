#include "SerSocket.hpp"
#include "Server.hpp"


void SerSocket::init(const char* port)
{
    struct addrinfo hints, *addr;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &addr))
        throw ServExcept("Server init getaddrinfo failed");

    this->_addr = addr;
    this->_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (this->_fd < 0)
    {
        freeaddrinfo(addr);
        throw ServExcept("Server socket creation failed");
    }
    
}

void SerSocket::action(uint32_t events) // accept new client
{
    (void)events;
    struct sockaddr_in client_addr;
    socklen_t size = sizeof client_addr;

    int newFd = accept(this->_fd, (struct sockaddr* )&client_addr, &size);
    if (newFd < 0)
        throw ServExcept("Server socket accept failed");

    if (fcntl(newFd, F_SETFL, O_NONBLOCK))
        throw ServExcept("Server socket fcntl failed");

    // ICI CONVERT IP TO STRING OR AILLEURS IDC
    

    Server::getInstance().newClient(client_addr, newFd);
}

struct addrinfo* SerSocket::getAddr()
{
    return this->_addr;
}

SerSocket::SerSocket()
{
}

SerSocket::~SerSocket()
{
    freeaddrinfo(this->_addr);
    close(this->_fd);
}