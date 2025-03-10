#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"


// Command::Command(CliSocket *client)
// {
//     this->_client = client;
//     _commands["PASS"] = new PassCommand();
//     _commands["CAP"] = new CapCommand();
//     _commands["NICK"] = new NickCommand();
//     _commands["USER"] = new UserCommand();
//     _commands["PING"] = new PingCommand();
//     _commands["INVITE"] = new InviteCommand();
//     _commands["JOIN"] = new JoinCommand();
//     _commands["KICK"] = new KickCommand();
//     _commands["MODE"] = new ModeCommand(); 
//     _commands["PART"] = new PartCommand();
//     _commands["PRIVMSG"] = new PrivmsgCommand();
//     _commands["QUIT"] = new QuitCommand();
//     _commands["TOPIC"] = new TopicCommand();

// }

Command::~Command() {}

void Command::sendMsg(Client *client, std::string msg)
{
    std::cout << "SERVER To FD: " << client->getFd() << " >> " << msg << std::endl;
    msg += "\r\n";
    send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}
