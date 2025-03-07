
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
#include <map>
#include <set>
#include "ServExcept.hpp"
// #include "MainSocket.hpp"
#include "SerSocket.hpp"
// #include "CliSocket.hpp"

class MainSocket;
class SerSocket;
class CliSocket;
class Channel;


class Server
{
private:
    Server();
    std::string _port;
    std::string _password;
    SerSocket _serSocket;
    bool _isLive;
    int _epfd;
    std::vector<CliSocket*> _clients;
    std::map<std::string, Channel*> _channelMap;
    std::set<int> _clientsFd;
    

    //size_t _maxClients;
    //bool _live;


    // add operators later



public:
    ~Server();

    void init(char **av);
    void boot(); // or foot() ?
    void subscribe(MainSocket &socket, int fd);
    void unsubscribe(int fd);
    void monitor();
    void newClient(sockaddr_in client_addr, int newFd);

    //remove things
    void removeClient(CliSocket *client);
    void removeFromFdList(int fd);


    // Chan interactions functions
    void newChan(CliSocket *client, std::string chanName, std::string key);

    bool isLive(); // THIS WILL NEED A SETTER GNAGNA JE SUIS GUILLAUME ET YANN JE CLC


    bool nickTaken(std::string nick);



    //getters
    std::string getPwd();
    std::vector<CliSocket *> getClients();
    const std::map<std::string, Channel*>& getOniChanUwU();
    std::set<int> getSharedChans(CliSocket *client);
    std::set<int> getFdList();


    static Server& getInstance();


    void shutdown();

    //UTILS
    void updateList(const std::string &oldmask, const std::string &newmask);


    //DEBUG

    void printClients(const std::vector<CliSocket*>& clients);
    void printChannels(const std::map<std::string, Channel*>& channelMap);
    static void printSet(const std::set<int>& list);


};

bool   sameString(const std::string s1, const std::string s2);