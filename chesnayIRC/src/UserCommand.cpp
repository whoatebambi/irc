#include "CliSocket.hpp"
#include "UserCommand.hpp"


void UserCommand::execute(const std::string &args, CliSocket *client)
{
    std::vector<std::string> arg = splitArgs(args);

    std::cout << "USER COMMAND EXECUTE DEBUG" << std::endl;

    if (arg.size() < 4)
        return sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str());

    if (!(client->getUsrName().empty()))
        return sendRpl(client, ERR_ALREADYREGISTERED, client->getNick().c_str());

    client->setUsrName(arg[0]);
    client->setRealName(arg[3]);
    //sendWelcome(client);
}
