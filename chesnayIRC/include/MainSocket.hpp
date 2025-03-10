#pragma once


#include <string>
#include <sys/types.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <vector>
// #include "Server.hpp"


class MainSocket
{
protected:
    int _fd;

public:

    int getFd(); 

    MainSocket();
    virtual ~MainSocket();

    virtual void action(uint32_t events) = 0;
};
