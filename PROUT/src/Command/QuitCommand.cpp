#include "QuitCommand.hpp"
#include "Server.hpp"
#include "CliSocket.hpp"


void QuitCommand::execute(const std::string &args, CliSocket *client)
{
	std::string source = client->getSource();

	std::set<int> pairs = Server::getInstance().getSharedChans(client);

	Server::getInstance().removeClient(client);

	std::string msg = source + " QUIT :Quit" + args;

	broadcast(pairs, msg);

	//sendMsg(client, msg);
}
