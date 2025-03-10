#pragma once

#include <netdb.h>
//#include "Server.hpp"
#include "MainSocket.hpp"

class SerSocket : public MainSocket
{
private:
    struct addrinfo *_addr;

    
public:
    struct addrinfo* getAddr(); // la position de l'etoile peut varier en fonction du multiverse


    void init(const char* port);

    void action(uint32_t events);

    SerSocket();
    ~SerSocket();
};

