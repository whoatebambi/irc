#include "CliSocket.hpp"
#include "PassCommand.hpp"
#include "Server.hpp"

void PassCommand::execute(const std::string &args, CliSocket *client)
{
    if ( args == (Server::getInstance().getPwd()))
    {
        if (!client->isAuth())
            client->setAuth();
        else
            return sendRpl(client, ERR_ALREADYREGISTERED, client->getNick().c_str());
    }
    else
    {
        sendRpl(client, ERR_PASSWDMISMATCH, client->getNick().c_str());
        Server::getInstance().removeClient(client);
    }
}
