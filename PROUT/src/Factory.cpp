#include "Factory.hpp"
#include "CliSocket.hpp"
#include "Server.hpp"

Factory::Factory(CliSocket *client)
{
    this->_client = client;
    _commands["PASS"] = new PassCommand();
    _commands["CAP"] = new CapCommand();
    _commands["NICK"] = new NickCommand();
    _commands["USER"] = new UserCommand();
    _commands["PING"] = new PingCommand();
    _commands["INVITE"] = new InviteCommand();
    _commands["JOIN"] = new JoinCommand();
    _commands["KICK"] = new KickCommand();
    _commands["MODE"] = new ModeCommand(); 
    _commands["PART"] = new PartCommand();
    _commands["PRIVMSG"] = new PrivmsgCommand();
    _commands["QUIT"] = new QuitCommand();
    _commands["TOPIC"] = new TopicCommand();
    
    // _commands["LIST"] = new ListCommand();
    // _commands["LUSER"] = new LuserCommand();
    // _commands["MOTD"] = new MotdCommand();
    // _commands["NAMES"] = new NamesCommand();
    // _commands["NOTICE"] = new NoticeCommand();
    // _commands["PONG"] = new PongCommand();
    // _commands["SERVER"] = new ServerCommand();
    // _commands["SETNAME"] = new SetnamesCommand();
    // _commands["SQUERY"] = new SqueryCommand();
    // _commands["WHO"] = new WhoCommand();
    // _commands["WHOIS"] = new WhoIsCommand();
}

Factory::~Factory()
{
    for (std::map<std::string, Command*>::iterator it = _commands.begin(); it != _commands.end(); ++it)
    {
        delete it->second;
    }
}

std::string Factory::ft_trim(const std::string &str)
{
    size_t end = str.find_last_not_of("\r\n");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

void Factory::parse(std::string &line)
{
    size_t pos = line.find(' ');
    std::string cmd = line.substr(0, pos);

    std::string args;
    if (pos != std::string::npos) 
        args = ft_trim(line.substr(pos + 1));
    else 
        args = "";

    if (_commands.find(cmd) != _commands.end())
        _commands[cmd]->execute(args, this->_client);
    else
        std::cout << "Unknown command: " << cmd << std::endl;
}
