#include "PingCommand.hpp"


void PingCommand::execute(const std::string &args, CliSocket *client)
{
    std::string trimmed = (args);

    sendMsg(client, "PONG " + trimmed);
}
