#include "CliSocket.hpp"
#include "CapCommand.hpp"

void CapCommand::execute(const std::string &args, CliSocket *client)
{
    if (args == "LS")
        sendMsg(client, "CAP * LS :");
    else if (args == "END")
        sendWelcome(client);

    // NEED TO BE IMPROVED To HANDLE OTHER ARGS
}