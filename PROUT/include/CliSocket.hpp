#pragma once

#include "MainSocket.hpp"
#include "Factory.hpp"
#include "Server.hpp"


class CliSocket : public MainSocket
{
private:
    std::string _ip;
    std::string _nickname;
    std::string _usrname;
    std::string _realname;
    std::string _saved;
    Factory _cmd;
    bool _auth;

    int _port;
public:

    void action(uint32_t events);

    std::string getNick();
    std::string getUsrName();
    std::string getRealName();
    std::string getSource();


    bool isAuth();
    

    void setAuth();
    void setNick(std::string arg);
    void setUsrName(std::string arg);
    void setRealName(std::string arg);
    
    
    //check list for chan join overload
    bool isInList(std::set<int> list);
    bool isInList(std::set<std::string> list);


    CliSocket(int fd, std::string ip, int port);

    ~CliSocket();
};

