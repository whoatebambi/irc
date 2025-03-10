#include "Channel.hpp"
#include "Command.hpp"
#include "CliSocket.hpp"


Channel::Channel(CliSocket *client, std::string name, std::string key)
{

    this->_name = name;
    this->_topic = "";
    this->_key = key;
    this->_topicEditor = "";
    this->_limClients = 0;
    this->_topicTime = 0;
    this->_creationTime = time(NULL);


    _modes['i'] = false; // whitelist mode
    _modes['t'] = false; // restricted topic
    if (!key.empty())
        _modes['k'] = true; // key protected
    else
        _modes['k'] = false;
    _modes['o'] = false; // op privilege A SUPPRIMER SUREMENT
    _modes['l'] = false; // user limit
    _modes['b'] = false;  //ban flag

    _clients.insert(client->getFd());
    _ops.insert(client->getSource());

}

Channel::~Channel()
{
}

bool Channel::canJoin(CliSocket *client, std::string key)
{
    if (client->isInList(this->_clients))
        return false;

    if (client->isInList(this->_banlist))
        return (Command::sendRpl(client, ERR_BANNEDFROMCHAN, client->getNick().c_str(), this->_name.c_str()), false);

    if (this->_modes['i'] == true)
        if (!(client->isInList(this->_whitelist)))
            return (Command::sendRpl(client, ERR_INVITEONLYCHAN, client->getNick().c_str(), this->_name.c_str()), false);

    if (this->_modes['k'] == true)
        if (key != this->_key)
            return (Command::sendRpl(client, ERR_BADCHANNELKEY, client->getNick().c_str(), this->_name.c_str()), false);
    if (this->_modes['l'] == true && this->_limClients <= this->_clients.size())
        return (Command::sendRpl(client, ERR_CHANNELISFULL, client->getNick().c_str(), this->_name.c_str()), false);
    return true;
}

void Channel::joinChan(CliSocket *client, std::string chanName, std::string key) //pb creation channel empty si par exempl /join ch1,,ch2
{
    if (!canJoin(client, key))
        return ;
    this->_clients.insert(client->getFd());
    Command::sendTopic(client, this);
    
    std::string msg = client->getSource() + " JOIN " + chanName;

    Command::broadcast(this->_clients, msg);
}


//overloads/////////////////////////

void Channel::removeFromList(std::set<int> &list, int fd)
{
    list.erase(fd);
}

void Channel::removeFromList(std::set<std::string> &list, std::string mask)
{
    list.erase(mask);
}

void Channel::removeFromList(std::set<int> &list, std::string mask)
{
    std::vector<CliSocket *> vecClient = Server::getInstance().getClients();
    std::vector<CliSocket *>::const_iterator it = vecClient.begin();
    for ( ; it != vecClient.end(); ++it)
    {
        if ((*it)->getSource() == mask)
        {
            list.erase((*it)->getFd());
            break ;
        }
    }
}


////////////////////////////////////

bool Channel::isEmpty()
{
    return (this->_clients.empty());
}

// GETTERS

std::set<int> &Channel::getMembers()
{
    return this->_clients;
}

std::set<std::string> &Channel::getBanList()
{
    return this->_banlist;
}

std::set<std::string> &Channel::getWhiteList()
{
    return this->_whitelist;
}

std::set<std::string> &Channel::getOpsList()
{
    return this->_ops;
}

std::string Channel::getName()
{
    return this->_name;
}

std::string Channel::getTopic()
{
    return this->_topic;
}

std::string Channel::getTopicEditor()
{
    return this->_topicEditor;
}

std::string Channel::getKey()
{
    return this->_key;
}

std::string &Channel::getFounderMask()
{
    return this->_founderMask;
}

std::string Channel::getLimitSize()
{
    std::ostringstream oss;
    std::string limitSize;
    oss << this->_limClients;  // Insert the integer into the stream
    limitSize = oss.str();
    if (limitSize == "0")
        limitSize = "";
    return limitSize;  // Extract the string
}


std::string Channel::getTopicTime()
{
    std::ostringstream ss;
    ss << this->_topicTime;
    return ss.str();
}

std::string Channel::getCreationTime()
{
    std::ostringstream ss;
    ss << this->_creationTime;
    return ss.str();
}



void Channel::setTopicInfo(std::string topic, std::string nickname)
{
    this->_topic = topic;
    this->_topicEditor = nickname;
    this->_topicTime = time(NULL);
}



bool Channel::checkMode(char c)
{
    return (this->_modes[c]);
}


/////////////////////////////// MODE FUNCTIONS ///////////////////////////////


void Channel::addOp(const std::string &mask, CliSocket *target, CliSocket *client)
{
    this->getOpsList().insert(mask);
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " +o " + target->getNick();
    Command::broadcast(this->getMembers(), msg);
}

void Channel::delOp(const std::string &mask, CliSocket *target, CliSocket *client)
{
    removeFromList(this->getOpsList(), mask);
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " -o " + target->getNick();
    Command::broadcast(this->getMembers(), msg);
}


void Channel::addLimit(const int &limit, CliSocket *client)
{
    this->_modes['l'] = true;
    this->_limClients = limit;
    std::stringstream ss;
    ss << limit;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " +l " + ss.str();
    Command::broadcast(this->getMembers(), msg);
}

void Channel::delLimit(CliSocket *client)
{
    this->_modes['l'] = false;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " -l";
    Command::broadcast(this->getMembers(), msg);
}


void Channel::addKey(const std::string &key, CliSocket *client)
{
    this->_modes['k'] = true;
    this->_key = key;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " +k " + key;
    Command::broadcast(this->getMembers(), msg);
}

void Channel::delKey(CliSocket *client)
{
    this->_modes['k'] = false;
    this->_key = "";
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " -k";
    Command::broadcast(this->getMembers(), msg);
}


void Channel::addInvOnly(CliSocket *client)
{
    this->_modes['i'] = true;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " +i";
    Command::broadcast(this->getMembers(), msg);
}

void Channel::delInvOnly(CliSocket *client)
{
    this->_modes['i'] = false;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " -i";
    Command::broadcast(this->getMembers(), msg);
}


void Channel::addNeedOpTopic(CliSocket *client)
{
    this->_modes['t'] = true;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " +t";
    Command::broadcast(this->getMembers(), msg);
}

void Channel::delNeedOpTopic(CliSocket *client)
{
    this->_modes['t'] = false;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " -t";
    Command::broadcast(this->getMembers(), msg);
}

void Channel::addBanList(CliSocket *target, CliSocket *client)
{
    this->getBanList().insert(target->getSource());
    std::cout << "================================ADDED TO BANLIST: " << target->getSource() << std::endl;
    this->_modes['b'] = true;
    if (target->isInList(this->getMembers()))
    {
        std::string msg = client->getSource() + " KICK " + this->getName() + " " + target->getNick() + " Banned from channel...";
        Command::broadcast(this->getMembers(), msg);
        msg = ":" + client->getNick() + " MODE " + this->getName() + " +b " + target->getNick();
        Command::broadcast(this->getMembers(), msg);
        removeFromList(this->getMembers(), target->getSource());
    }
}

void Channel::delBanList(CliSocket *target, CliSocket *client)
{
    removeFromList(this->getBanList(), target->getSource());
    if(this->getBanList().size() == 0)
        this->_modes['b'] = false;
    std::string msg = ":" + client->getNick() + " MODE " + this->getName() + " -b " + target->getNick();
    Command::broadcast(this->getMembers(), msg);
}

std::string Channel::modeString(void)
{
    std::string modeString = "+";
    if(this->_modes['b'])
        modeString +="b";
    if(this->_modes['i'])
        modeString +="i";
    if(this->_modes['t'])
        modeString +="t";
    if(this->_modes['k'])
        modeString +="k";
    if(this->_modes['o'])
        modeString +="o";
    if(this->_modes['l'])
        modeString +="l";
    return modeString;
}


